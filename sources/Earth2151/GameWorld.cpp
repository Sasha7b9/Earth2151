// 2025/01/10 19:52:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "GameWorld.h"
#include "Objects/World/Landscape.h"
#include "Objects/Terrain.h"
#include "Graphics/Primitives.h"
#include "Objects/Structures/StructureTask.h"
#include "Objects/UCreator.h"
#include "Cameras.h"
#include "Interface/Widgets/GameCursor.h"
#include "Interface/GUI.h"
#include "Input/Mouse.h"
#include "Objects/World/Sun.h"
#include "Graphics/Particles.h"
#include "Earth2151.h"


int GameWorld::current = 0;
GameWorld *GameWorld::worlds[4] = { nullptr, nullptr, nullptr, nullptr };


GameWorld::GameWorld(pchar name) : World(name)
{
}


GameWorld *GameWorld::Current()
{
    return worlds[current];
}


void GameWorld::Create(int num, pchar name)
{
    if (num >= 0 && num < 4)
    {
        worlds[num] = new GameWorld(name);
    }
    else
    {
        LOG_ERROR("Bad number world - %d", num);
    }
}


void GameWorld::Set(int num)
{
    current = num;
}


WorldResult GameWorld::PreprocessWorld()
{
    WorldResult result = World::PreprocessWorld();

    if (result != kWorldOkay)
    {
        return result;
    }

    SetWorldCamera(TheCameraRTS);

    return kWorldOkay;
}


void GameWorld::MoveWorld()
{
    World::MoveWorld();

    TheWorldMgr->GetWorld()->FinishWorldBatch();

    if (Earth2151::target_destroed)
    {
        Earth2151::target_destroed = false;
    }

    UCreator::Update();

    Sun::Update();
}


void GameWorld::RunOnFirstFrame()
{
    Particles::Init();

    ChangeCursorPosition(1.0f, 1.0f);           // \todo Затычка чтобы курсор установился на середину экрана при запуске
    ChangeCursorPosition(-1.0f, -1.0f);
}


void GameWorld::BeginRendering(ToneMapper *mapper)
{
//    mapper->DisableExposure();
//    mapper->DisableAdaptation();
//    mapper->DeactivateToneMapper();

    World::BeginRendering(nullptr);
}


LocatorMarker *GameWorld::FindSpectatorLocator(const Zone *zone)
{
    const Marker *marker = zone->GetFirstMarker();
    while (marker)
    {
        if (marker->NodeEnabled())
        {
            if (marker->markerType == kMarkerLocator)
            {
                LocatorMarker *locator = (LocatorMarker *)marker;
                if (locator->locatorType == kLocatorSpectator)
                {
                    return (locator);
                }
            }
        }

        // Получить следующий маркер в списке.

        marker = marker->GetNextListElement();
    }

    // Посмотрите на все подзоны.

    const Zone *subzone = zone->GetFirstSubzone();
    while (subzone)
    {
        LocatorMarker *locator = FindSpectatorLocator(subzone);
        if (locator)
        {
            return locator;
        }

        subzone = subzone->GetNextListElement();
    }

    return nullptr;
}


Node *GameWorld::FindNodeByName(pchar node_name)
{
    return GetRootNode()->FindFirstNodeByName(node_name);
}


void GameWorld::ChangeCursorPosition(float deltaX, float deltaY)
{
    TheGameCursor->position.x += deltaX * 3.0F;
    TheGameCursor->position.y += deltaY * 3.0F;

    //    float displayWidth = (float)DisplayMgr::self->GetDisplayWidth();
    //    float displayHeight = (float)DisplayMgr::self->GetDisplayHeight();

    float displayWidth = (float)TheEngine->GetOutputResolution().x;
    float displayHeight = (float)TheEngine->GetOutputResolution().y;

    TheGameCursor->position.x = FmaxZero(TheGameCursor->position.x);
    TheGameCursor->position.y = FmaxZero(TheGameCursor->position.y);

    TheGameCursor->position.x = Fmin(displayWidth - 1, TheGameCursor->position.x);
    TheGameCursor->position.y = Fmin(displayHeight - 1, TheGameCursor->position.y);

    Vector2D cursorSize = TheGameCursor->GetWidgetSize();
    float posX = TheGameCursor->position.x - 0.5F * cursorSize.x;
    float posY = TheGameCursor->position.y - 0.5F * cursorSize.y;

    TheGameCursor->SetWidgetPosition({ (float)((int)posX), (float)((int)posY) });
    TheGameCursor->InvalidateWidget();

    if (TheGUI)
    {
        TheMouse->ChangePos((int)posX, (int)posY);
    }
}
