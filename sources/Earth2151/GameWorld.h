// 2025/01/10 19:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Earth2150Types.h"


enum
{
    kCollisionLandscape = (1 << 20),
    kCollisionGameObject = (1 << 30)
};


class Landscape;


class GameWorld : public World
{
public:

    virtual ~GameWorld();

    Node *FindNodeByName(pchar);

    virtual ResourceResult PreprocessWorld() override;

    virtual void BeginRendering(ToneMapper *) override;

    void ChangeCursorPosition(float deltaX, float deltaY);

    LocatorMarker *FindSpectatorLocator(const Zone *);

    // int = [0...3]
    static void Create(int, pchar name);

    // Без первого подчёркивания ошибка компиляции на Linux
    static void _DestroyAll();

    // int - от нуля до 3 для выбора мира из worlds
    static void Set(int);

    static int Get();

    static GameWorld *Current();

    static void UpdateAll();

    Landscape *landscape = nullptr;
    Level2150 *level = nullptr;

private:

    GameWorld(int num_world, pchar name);

    // Здесь хранятся четыре мира. В нулевом элементе - миссия; в остальных - базы в соответствии с Race::E
    static GameWorld *worlds[4];

    void RunOnFirstFrame();

    void Update();

    int num_world;

    static int current;
};
