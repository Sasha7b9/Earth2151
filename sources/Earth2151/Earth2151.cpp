// 2022/12/18 11:57:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GameWorld.h"
#include "Cameras.h"
#include "Input/Input.h"
#include "Input/Mouse.h"
#include "Interface/GUI.h"
#include "Earth2150/Earth2150Tool.h"
#include "Objects/World/Landscape.h"
#include "Graphics/PoolTextures.h"
#include "Earth2150/Files/TexLand2150.h"
#include "Objects/World/Sun.h"
#include "Earth2150/Files/Model2150.h"
#include "Earth2150/Files/TexMesh2150.h"
#include "Graphics/Primitives.h"
#include "Graphics/Particles.h"
#include "Objects/Objects.h"
#include "Objects/World/Tunnels.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Clock.h"
#include "Earth2151.h"
#include "Interface/Menu/Menu.h"
#include "Utils/Local.h"
#include "Editor/Editor.h"


using namespace Pi;


PI_ENTER_POINT(Earth2151, false)


Earth2151 *TheEarth2151 = nullptr;
Level2150 *TheLevel = nullptr;
Parameters2150 *TheParameter = nullptr;


bool Earth2151::target_destroed = false;


Earth2151::Earth2151() : Global<Earth2151>(TheEarth2151)
{
    TheWorldMgr->SetWorldCreator(&CreateWorld);

    TheEngine->AddCommand(&gizmoCommand.command);
    TheEngine->AddCommand(&cameraCommand.command);

    TheInterfaceMgr->SetInputManagementMode(kInputManagementAutomatic);

    Local::Init();

    Input::Init();

    new Mouse();

    EMenu::Init();

    TheInterfaceMgr->AddWidget(TheStartWindow);

//    LoadLevel();

//    TheEditor->LoadLevel("!BaseED.lnd");
}


void Earth2151::LoadLevel()
{
    uint time = UCOUNT_MS;

    TheWorldMgr->LoadWorld("world");

    TheMessageMgr->BeginSinglePlayerGame();

    TheLevel = new Level2150{ RESOURCE_PATH("Levels/!BaseED") };

    CreateLandscape();

    TheParameter = new Parameters2150{ RESOURCE_PATH("parameters/EARTH2150.par").c_str() };

    TheParameter->Save();

    TexMesh2150::Init();

    CreateGameObjects();

    {
        TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(new WorldGizmo(Landscape::GetSize().x, 0.1f));

        float ambient = 0.5f;

        TheWorldMgr->GetWorld()->GetRootNode()->GetObject()->SetAmbientLight({ ambient, ambient, ambient, 0.0f });
    }

    Sun::Init();

    new GUI();

    CameraSpecatator::Create(TheGameWorld->FindSpectatorLocator(TheWorldMgr->GetWorld()->GetRootNode()));

    CameraRTS::Create(TheGameWorld->FindSpectatorLocator(TheWorldMgr->GetWorld()->GetRootNode()));

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

    LOG_WRITE("Time load game %f sec", (float)(UCOUNT_MS - time) / 1e3f);
}


Earth2151::~Earth2151()
{
    Local::DeInit();

    EMenu::DeInit();

    Tunnels::Destroy();

    Landscape::Destroy();

    SAFE_DELETE(TheParameter);

    SAFE_DELETE(TheMouse);

    Model2150::Destroy();

    SAFE_DELETE(TheCameraSpectator);

    SAFE_DELETE(TheCameraRTS);

    SAFE_DELETE(TheGUI);

    SAFE_DELETE(TheEditor);

    TexLand2150::Destroy();

    TexMesh2150::DeInit();

    PoolTextures::Destruct();

    TheMessageMgr->EndGame();

    TheWorldMgr->UnloadWorld();
    TheWorldMgr->SetWorldCreator(nullptr);
    TheMessageMgr->EndGame();

    SAFE_DELETE(TheLevel);
}


void Earth2151::CreateLandscape()
{
    uint time = UCOUNT_MS;

//    Earth2150::Unzipper::UnzipAllWD(RESOURCE_PATH("WDFiles/"));

    PoolTextures::Construct();

    {
        if (Earth2150::Reader::ReadLand(*TheLevel))
        {
            Landscape::Create(*TheLevel);

            TheLevel->ReadObjects();
        }
    }

    LOG_WRITE("Time create landscape %f sec", (float)(UCOUNT_MS - time) / 1e3f);

//    CreateAllModels();
}


void Earth2151::CreateAllModels()
{
    Map<FileReference> file_map;

    FileMgr::BuildFileMap(RESOURCE_PATH("meshes/"), &file_map, [](pchar name, uint flags, const void *)
    {
        return SU::EndsWith(name, ".msh");
    });

    int i = 0;

    if (!file_map.Empty())
    {
        FileReference *element = file_map.GetFirstMapElement();

        while (element)
        {
            if ((element->GetFileFlags() & kFileDirectory) == 0)
            {
                String<> file_name(element->GetFileName());

                Model2150 *model = new Model2150(file_name.c_str());

                if (model->IsModel())
                {
                    Node *node = model->CreateModel();

                    node->SetNodePosition({ (float)(i % 35) * 5.0f, (float)(i / 35) * 5.0f, 50.0f });

                    node->AppendNewSubnode(new WorldGizmo(1.0f, 0.01f));

                    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(node);

                    i++;
                }
            }

            element = element->GetNextMapElement();
        }
    }
}


void Earth2151::CreateGameObjects()
{
    uint time = UCOUNT_MS;

    int counter = 0;

    for each(LObject &obj in TheLevel->objects.objects)
    {
        obj.jobCreateObject.ExecuteJob();

//        TheWorldMgr->GetWorld()->SubmitWorldJob(&obj.jobCreateObject);

        counter++;
    }

//    TheWorldMgr->GetWorld()->FinishWorldBatch();

    time = UCOUNT_MS - time;

    LOG_WRITE("Time find entities %f sec", TheParameter->time_find_us / 1e6f);

    LOG_WRITE("%d objects for %d ms, %f ms/obj", counter, time, (float)time / (counter + 1));
}


void Earth2151::HandleGizmoCommand(Command *, pchar text)
{
    g_showing_gizmo = !g_showing_gizmo;

    Node *node = TheWorldMgr->GetWorld()->GetRootNode();
    Node *subnode = node;

    do
    {
        if (subnode->GetNodeType() == 'wlgz')
        {
            g_showing_gizmo ? subnode->EnableNode() : subnode->DisableNode();
            subnode->InvalidateNode();
        }

        subnode = node->GetNextTreeNode(subnode);

    } while (subnode);
}


void Earth2151::HandleCameraCommand(Command *, pchar)
{
    World *world = TheWorldMgr->GetWorld();

    if (world->GetWorldCamera() == TheCameraSpectator)
    {
        world->SetWorldCamera(TheCameraRTS);
    }
    else
    {
        world->SetWorldCamera(TheCameraSpectator);
    }
}


World *Earth2151::CreateWorld(pchar name, void *)
{
    return new GameWorld(name);
}


void Earth2151::UnloadWorld()
{
    Particles::DeInit();
    TheWorldMgr->UnloadWorld();
    TheMessageMgr->EndGame();
}


void MovementAction::HandleEngage()
{
    World *world = TheWorldMgr->GetWorld();

    if (world)
    {
        TheCameraSpectator->spectatorFlags |= movementFlag;
    }
}


void MovementAction::HandleDisengage()
{
    World *world = TheWorldMgr->GetWorld();

    if (world)
    {
        TheCameraSpectator->spectatorFlags &= ~movementFlag;
    }
}


void Earth2151::EscapeCallback(void *)
{
    TheInterfaceMgr->AddWidget(TheGameMenuWindow);

    TheEarth2151->Pause();
}


void Earth2151::Pause()
{
    in_paused = true;

    Clock::Pause();
}


void Earth2151::Resume()
{
    in_paused = false;

    Clock::Resume();
}


bool Earth2151::InPaused()
{
    return in_paused;
}


String<> Earth2151::ResourceFile(pchar name)
{
    if (TypeGame::IsEftBP())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150/" + name;
    }
    else if (TypeGame::IsMP())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150-MP/" + name;
    }
    else if (TypeGame::IsLS())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150-LS/" + name;
    }

    return "";
}
