// 2025/04/10 16:20:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Mission.h"


std::map<int, Mission *> Mission::missions;


void Mission::Register(int id, pchar file_level, pchar file_script, pchar briefing, uint flags, int longitude, int latitude, int d1, int d2, int d3, int id_next1, int id_next2, int id_next3, int id_next4)
{
    missions[id] = new Mission{ id, file_level, file_script, briefing, flags, longitude, latitude, d1, d2, d3, id_next1, id_next2, id_next3, id_next4 };
}


void Mission::Destroy()
{
    for (auto mission : missions)
    {
        delete mission.second;
    }

    missions.clear();
}


const Mission *Mission::Get(int id)
{
    auto elem = missions.find(id);

    return elem->second;
}
