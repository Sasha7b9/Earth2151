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

    void CreateLandscape();

    void CreateGameObjects();

    void LoadLevel(pchar);
};


extern Game *TheGame;
