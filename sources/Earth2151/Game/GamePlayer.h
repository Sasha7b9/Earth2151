// 2025/04/10 17:23:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/GameTypes.h"


struct PlayerType
{
    enum E
    {
        Local,
        AI,
        Count
    };
};


class GamePlayer
{
public:

    GamePlayer(int id, Race::E, PlayerType::E, pchar script);

    static GamePlayer *Create(int id, Race::E, PlayerType::E, pchar file_script);

    static void Destroy();

private:

    int id;
    Race::E race;
    PlayerType::E type;
    String<> file_script;

    static std::map<int, GamePlayer *> players;
};
