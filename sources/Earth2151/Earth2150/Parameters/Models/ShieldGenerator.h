// 2025/03/05 20:13:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct ShieldGenerator : public TypelessEntity
{
    ShieldGenerator(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        shield_cost = reader.ReadInt();
        shield_value = reader.ReadInt();
        reload_time = reader.ReadInt();
        shield_mesh_name = reader.ReadString(4);
        shield_mesh_view_index = reader.ReadInt();
    }

    int shield_cost;
    int shield_value;
    int reload_time;
    String<> shield_mesh_name;
    int shield_mesh_view_index;
};
