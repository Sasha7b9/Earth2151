// 2025/03/05 19:59:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/EquipableEntity.h"


struct Platoon : public EquipableEntity
{
    Platoon(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        EquipableEntity(name, researhces, type, reader)
    {

    }
};
