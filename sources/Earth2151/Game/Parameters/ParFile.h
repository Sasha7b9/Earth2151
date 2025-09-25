// 2025/03/05 09:04:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Game/Parameters/Models/Abstracts/Entity.h"


struct ResearchType
{
    enum E
    {
        Chassis,
        Weapon,
        Ammo,
        Special
    };
};


struct Research
{
    int id;
    Faction faction;
    int compaign_cost;
    int skirmish_cost;
    int compaign_time;
    int skirmish_time;
    String<> name;
    String<> video;
    ResearchType::E type;
    String<> mesh;
    int mesh_params_index;
    Array<int> required_research;

    void Read(FileReader &);
};
