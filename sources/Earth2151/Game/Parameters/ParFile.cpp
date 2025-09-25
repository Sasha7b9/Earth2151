// 2025/03/05 09:04:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Parameters/ParFile.h"


void Research::Read(FileReader &reader)
{
    id = reader.ReadInt();
    faction.value = (Faction::E)reader.ReadInt();
    compaign_cost = reader.ReadInt();
    skirmish_cost = reader.ReadInt();
    compaign_time = reader.ReadInt();
    skirmish_time = reader.ReadInt();
    name = reader.ReadString();
    video = reader.ReadString();
    type = (ResearchType::E)reader.ReadInt();
    mesh = reader.ReadString();
    mesh_params_index = reader.ReadInt();

    int count = reader.ReadInt();

    for (int i = 0; i < count; i++)
    {
        required_research.AppendArrayElement(reader.ReadInt());
    }
}
