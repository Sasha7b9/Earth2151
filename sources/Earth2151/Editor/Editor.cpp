// 2025/03/20 15:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Editor/Editor.h"
#include "Interface/GUI.h"
#include "Objects/World/Sun.h"
#include "Cameras.h"
#include "GameWorld.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Interface/Menu/LoadLevelWindow.h"
#include "Earth2150/Files/TexMesh2150.h"
#include "Graphics/Primitives.h"
#include "Objects/World/Landscape.h"
#include "Graphics/PoolTextures.h"
#include "Objects/World/Tunnels.h"


Editor *TheEditor = nullptr;


Editor::Editor() : Global<Editor>(TheEditor)
{
    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheWorldMgr->LoadWorld("world");

    Sun::Init();

    TheMessageMgr->BeginSinglePlayerGame();

    if (!TheCameraSpectator)
    {
        CameraSpecatator::Create(GameWorld::Current()->FindSpectatorLocator(TheWorldMgr->GetWorld()->GetRootNode()));
    }

    TheGUI->Show();

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);
}


void Editor::LoadLevel(pchar name)
{
    UnloadLevel();

    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheWorldMgr->LoadWorld("world");

    Sun::Init();

    TheMessageMgr->BeginSinglePlayerGame();

    if (!TheCameraSpectator)
    {
        CameraSpecatator::Create(GameWorld::Current()->FindSpectatorLocator(TheWorldMgr->GetWorld()->GetRootNode()));
    }

    TheGUI->Show();

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

    String<> full_name = RESOURCE_PATH("Levels/") + name;

    full_name[full_name.GetStringLength() - 4] = '\0';

    TheLevel = new Level2150{ full_name };

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
}


void Editor::UnloadLevel()
{
    Tunnels::Destroy();

    Landscape::Destroy();

    SAFE_DELETE(TheParameter);

    SAFE_DELETE(TheLevel);

    Model2150::Destroy();

    TexLand2150::Destroy();

    TexMesh2150::DeInit();

    PoolTextures::Destruct();

    TheWorldMgr->UnloadWorld();

    if (ThePanelMap)
    {
        ThePanelMap->ClearMap();
    }
}


void Editor::EscapeCallback(void *)
{
    if (TheEarth2151->InPaused())
    {
        TheInterfaceMgr->RemoveWidget(TheGameMenuWindow);

        TheEarth2151->Resume();
    }
    else
    {
        TheInterfaceMgr->AddWidget(TheGameMenuWindow);

        TheEarth2151->Pause();
    }
}


void Editor::OpenWindowLoadLevel()
{
    TheInterfaceMgr->AddWidget(TheLoadLevelWindow);

    TheInterfaceMgr->RemoveWidget(TheLoadLevelWindow);

    TheInterfaceMgr->AddWidget(TheLoadLevelWindow);
}


void Editor::CreateLandscape()
{
    uint time = UCOUNT_MS;

    PoolTextures::Construct();

    {
        if (Earth2150::Reader::ReadLand(*TheLevel))
        {
            Landscape::Create(*TheLevel);

            TheLevel->ReadObjects();
        }
    }

    LOG_WRITE("Time create landscape %f sec", (float)(UCOUNT_MS - time) / 1e3f);
}

void Editor::CreateGameObjects()
{
    uint time = UCOUNT_MS;

    int counter = 0;

    for (LObject &obj : TheLevel->objects.objects)
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
