// 2025/01/10 19:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


enum
{
    kCollisionLandscape = (1 << 20),
    kCollisionGameObject = (1 << 30)
};


class GameWorld : public World
{
public:

    virtual ~GameWorld() override { };

    Node *FindNodeByName(pchar);

    virtual ResourceResult PreprocessWorld() override;

    virtual void BeginRendering(ToneMapper *) override;

    void ChangeCursorPosition(float deltaX, float deltaY);

    LocatorMarker *FindSpectatorLocator(const Zone *);

    // int = [0...3]
    static void Create(int, pchar name);

    // int - от нуля до 3 для выбора мира из worlds
    static void Set(int);

    static GameWorld *Current();

private:

    GameWorld(pchar name);

    // Здесь хранятся четыре мира. В нулевом элементе - миссия; в остальных - базы в соответствии с Race::E
    static GameWorld *worlds[4];

    virtual void MoveWorld() override;

    void RunOnFirstFrame();

    static int current;
};
