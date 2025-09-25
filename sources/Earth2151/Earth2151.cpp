// 2022/12/18 11:57:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GameWorld.h"
#include "Input/Input.h"
#include "Interface/GUI.h"
#include "Objects/World/Landscape.h"
#include "Graphics/PoolTextures.h"
#include "Game/Files/TexMesh2150.h"
#include "Graphics/Primitives.h"
#include "Graphics/Particles.h"
#include "Objects/World/Tunnels.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Earth2151.h"
#include "Interface/Menu/Menu.h"
#include "Utils/Locale.h"
#include "Editor/Editor.h"
#include "Game/Scripting/ScriptEngine.h"
#include "Game/Game.h"
#include "Game/GamePlayer.h"


using namespace Pi;


PI_ENTER_POINT(Earth2151, false)


Earth2151 *TheEarth2151 = nullptr;
Parameters2150 *TheParameter = nullptr;


Earth2151::Earth2151() : Global<Earth2151>(TheEarth2151)
{
    TheWorldMgr->SetWorldCreator(&CreateWorld);

    TheEngine->AddCommand(&gizmoCommand.command);
    TheEngine->AddCommand(&cameraCommand.command);

    TheInterfaceMgr->SetInputManagementMode(kInputManagementAutomatic);

    new ScriptEngine();

    Local::Init();

    Input::Init();

    new Mouse();

    EMenu::Init();

    TheInterfaceMgr->AddWidget(TheStartWindow);

    new GUI();
}


Earth2151::~Earth2151()
{
    Local::DeInit();

    EMenu::DeInit();

    GameWorld::_DestroyAll();

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

    SAFE_DELETE(TheScriptEngine);

    TheMessageMgr->EndGame();

    TheWorldMgr->UnloadWorld();
    TheWorldMgr->SetWorldCreator(nullptr);
    TheMessageMgr->EndGame();

    GamePlayer::Destroy();

    SAFE_DELETE(TheGame);
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
                String<> file_name(element->fileName);

                Model2150 *model = new Model2150(file_name.c_str());

                if (model->IsModel())
                {
                    Node *node = model->CreateModel();

                    node->SetNodePosition({ (float)(i % 35) * 5.0f, (float)(i / 35) * 5.0f, 50.0f });

                    node->AppendNewSubnode(new WorldGizmo(1.0f, 0.01f));

                    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(node);

                    i++;
                }

                delete model;
            }

            element = element->GetNextMapElement();
        }
    }
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
    GameWorld::Create(0, name);

    GameWorld::Set(0);

    return GameWorld::Current();
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
    if (NameGame().IsEftBP())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150/" + name;
    }
    else if (NameGame().IsMP())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150-MP/" + name;
    }
    else if (NameGame().IsLS())
    {
        return String<>(TheResourceMgr->DataCatalog()->GetRootPath()) + "Earth2150-LS/" + name;
    }

    return "";
}


void Earth2151::ApplicationTask()
{
    if (TheGame)
    {
        TheGame->Update();
    }
}
