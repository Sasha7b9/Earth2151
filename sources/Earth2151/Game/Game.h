// 2025/03/24 17:38:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Game : public Global<Game>
{
public:

    Game();

    virtual ~Game();

    void LoadBase(int num_world, int id_mission, int id_owner);

    void UnloadLevel();

    void Update();

private:

    static void EscapeCallback(void *);

    static World *CreateWorld(pchar name, void *);

    void CreateLandscape(int num_world);

    void CreateGameObjects();

    void LoadLevel(int num_world, pchar);
};


extern Game *TheGame;


struct TypeGame
{
    enum E
    {
        Campaign,
        Tutorial,
        Skirmish,
        Count
    };

    static bool IsCampaign()
    {
        return current == Campaign;
    }
    static bool IsTutorial()
    {
        return current == Tutorial;
    }
    static bool IsSkirmish()
    {
        return current == Skirmish;
    }
    static void Set(E v)
    {
        current = v;
    }

private:

    static E current;
};


struct Race
{
    enum E
    {
        UCS = 1,
        ED = 2,
        LC = 3,
        Count
    };
};
