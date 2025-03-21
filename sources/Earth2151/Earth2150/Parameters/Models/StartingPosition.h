// 2025/03/05 17:44:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/EquipableEntity.h"


struct StartingPosition : public EquipableEntity
{
    StartingPosition(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        EquipableEntity(name, researhces, type, reader)
    {
        position_type = reader.ReadInt();
    }

    int position_type;
};

