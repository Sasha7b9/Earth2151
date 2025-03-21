// 2025/03/05 13:15:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/EquipableEntity.h"


struct VerticalTransporter : public EquipableEntity
{
    VerticalTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        EquipableEntity(name, researhces, type, reader)
    {
        vehicle_speed = reader.ReadInt();
        vertical_vehicle_animation_type = reader.ReadInt();
    }

    int vehicle_speed;
    int vertical_vehicle_animation_type;
};
