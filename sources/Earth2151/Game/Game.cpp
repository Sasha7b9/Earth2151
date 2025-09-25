// 2025/03/24 17:38:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game.h"
#include "Clock.h"
#include "Cameras.h"
#include "Earth2151.h"
#include "Game/Files/TexMesh2150.h"
#include "Graphics/Primitives.h"
#include "Objects/World/Landscape.h"
#include "Objects/World/Sun.h"
#include "GameWorld.h"
#include "Graphics/PoolTextures.h"
#include "Interface/GUI.h"
#include "Game/Scripting/ScriptEngine.h"
#include "Game/Scripting/ScriptStorage.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Game/GamePlayer.h"


Game *TheGame = nullptr;
TypeGame::E TypeGame::current = TypeGame::Count;


Game::Game() : Global<Game>(TheGame)
{
    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheWorldMgr->SetWorldCreator(&CreateWorld);

    TheParameter = new Parameters2150{ RESOURCE_PATH("parameters/EARTH2150.par").c_str() };

    TheParameter->Save();

    TexMesh2150::Init();

    TheGUI->Show();

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

    SAFE_DELETE(TheMission);
    SAFE_DELETE(TheCampaign);

    if (TypeGame::IsCampaign())
    {
        TheCampaign = TheScriptStorage->GetObject(TypeCampaign::FileCampaign())->ToCampaign();
        TheCampaign->Reset();
    }
    else if (TypeGame::IsTutorial())
    {
        TheMission = TheScriptStorage->GetObject(TypeCampaign::FileTutorial())->ToMission();
        TheMission->Reset();
    }

    TheMessageMgr->BeginSinglePlayerGame();
}


Game::~Game()
{
   Mission::Destroy();

    UnloadLevel();
}


World *Game::CreateWorld(pchar name, void *cookie)
{
    int *num_world = (int *)cookie;

    GameWorld::Create(*num_world, name);

    GameWorld::Set(*num_world);

    return GameWorld::Current();
}


void Game::LoadLevel(int num_world, pchar name_level)
{
//    uint time = UCOUNT_MS;

    if (!TheCameraRTS)
    {
        new CameraRTS();
    }

    TheWorldMgr->SetWorld(nullptr);

    TheWorldMgr->LoadWorld("world", false, &num_world);

    Sun::Init();

    GameWorld::Current()->level = new Level2150{ RESOURCE_PATH("Levels/") + name_level };

    CreateLandscape(num_world);

    CreateGameObjects();

    World *world = TheWorldMgr->GetWorld();

    {
        world->GetRootNode()->AppendNewSubnode(new WorldGizmo(LANDSCAPE->GetSize().x, 0.1f));

        float ambient = 0.5f;

        world->GetRootNode()->GetObject()->SetAmbientLight({ ambient, ambient, ambient, 0.0f });
    }

    if (!TheCameraSpectator)
    {
        CameraSpecatator::Create(GameWorld::Current()->FindSpectatorLocator(world->GetRootNode()));
    }

    TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

//    LOG_WRITE("Time load game %f sec", (float)(UCOUNT_MS - time) / 1e3f);
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


void Game::CreateLandscape(int num_world)
{
    PoolTextures::Construct();

    {
        if (Earth2150::Reader::ReadLand(*GameWorld::Current()->level))
        {
            Landscape::Create(*GameWorld::Current()->level, GameWorld::Current());

            GameWorld::Current()->level->ReadObjects();
        }
    }
}


void Game::CreateGameObjects()
{
    uint time = UCOUNT_MS;

    int counter = 0;

    for (LObject &obj : GameWorld::Current()->level->objects.objects)
    {
        obj.jobCreateObject.ExecuteJob();

//        TheWorldMgr->GetWorld()->SubmitWorldJob(&obj.jobCreateObject);

        counter++;
    }

//    TheWorldMgr->GetWorld()->FinishWorldBatch();

    time = UCOUNT_MS - time;

//    LOG_WRITE("Time find entities %f sec", TheParameter->time_find_us / 1e6f);

//    LOG_WRITE("%d objects for %d ms, %f ms/obj", counter, time, (float)time / (counter + 1));
}


void Game::Update()
{
    if (TheCampaign)
    {
        TheCampaign->Update();
    }

    if (TheMission)
    {
        TheMission->Update();
    }

    GameWorld::UpdateAll();
}


void Game::LoadBase(int num_world, int id_mission, int id_owner)
{
    const Mission *mission = Mission::Get(id_mission);

    LoadLevel(num_world, SU::RemoveQuotes(mission->file_level));

    GameWorld::Set(GamePlayer::GetLocalPlayer()->GetRace());
}
