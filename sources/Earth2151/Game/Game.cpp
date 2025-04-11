// 2025/03/24 17:38:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game.h"
#include "Clock.h"
#include "Cameras.h"
#include "Earth2151.h"
#include "Earth2150/Files/TexMesh2150.h"
#include "Graphics/Primitives.h"
#include "Objects/World/Landscape.h"
#include "Objects/World/Sun.h"
#include "GameWorld.h"
#include "Graphics/PoolTextures.h"
#include "Interface/GUI.h"
#include "Earth2150/Scripting/ScriptEngine.h"
#include "Earth2150/Scripting/ScriptStorage.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Game/Mission.h"


Game *TheGame = nullptr;


Game::Game() : Global<Game>(TheGame)
{
    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheParameter = new Parameters2150{ RESOURCE_PATH("parameters/EARTH2150.par").c_str() };

    TheParameter->Save();

    TexMesh2150::Init();

    TheGUI->Show();

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

    TheCampaign = TheScriptStorage->GetObject(TypeCampaign::FileCampaign())->ToCampaign();

    TheCampaign->Reset();

    TheMessageMgr->BeginSinglePlayerGame();
}


Game::~Game()
{
    Mission::Destroy();

    UnloadLevel();
}


void Game::LoadLevel(pchar name_level)
{
    uint time = UCOUNT_MS;

    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheWorldMgr->LoadWorld("world");

    Sun::Init();

    TheLevel = new Level2150{ RESOURCE_PATH("Levels/") + name_level };

    CreateLandscape();

    CreateGameObjects();

    World *world = TheWorldMgr->GetWorld();

    {
        world->GetRootNode()->AppendNewSubnode(new WorldGizmo(Landscape::GetSize().x, 0.1f));

        float ambient = 0.5f;

        world->GetRootNode()->GetObject()->SetAmbientLight({ ambient, ambient, ambient, 0.0f });
    }

    if (!TheCameraSpectator)
    {
        CameraSpecatator::Create(GameWorld::Current()->FindSpectatorLocator(world->GetRootNode()));
    }

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

    LOG_WRITE("Time load game %f sec", (float)(UCOUNT_MS - time) / 1e3f);
}



void Game::UnloadLevel()
{

}


void Game::EscapeCallback(void *)
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


void Game::CreateLandscape()
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


void Game::CreateGameObjects()
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


void Game::Update()
{
    TheCampaign->Update();
}


void Game::LoadBase(int num_world, int id_mission, int id_owner)
{
    LOG_WRITE("\"%s\" num_world=%d id_mission=%d id_owner=%d", __FUNCTION__, num_world, id_owner, id_owner);

    /*
        Базы загружаются в один и тот же мир.
        1-я база на высоте 0
        2-я база на высоте 100
        3-я база на высоте 200

        Игровой уровень находится в том же самом мире. Его высота - 300
    */

    const Mission *mission = Mission::Get(id_mission);

    static bool first = true;
    if (first)
    {
        first = false;
        LoadLevel(SU::RemoveQuotes(mission->file_level));
    }
}
