// 2025/04/10 17:23:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/GamePlayer.h"


std::map<int, GamePlayer *> GamePlayer::players;


GamePlayer::GamePlayer(int _id, Race::E _race, PlayerType::E _type, pchar _script) :
    id(_id), race(_race), type(_type), file_script(_script)
{

}


GamePlayer *GamePlayer::Create(int id, Race::E race, PlayerType::E type, pchar file_script)
{
    players[id] = new GamePlayer{ id, race, type, file_script };

    return players[id];
}


void GamePlayer::Destroy()
{
    for (auto player : players)
    {
        delete player.second;
    }

    players.clear();
}
