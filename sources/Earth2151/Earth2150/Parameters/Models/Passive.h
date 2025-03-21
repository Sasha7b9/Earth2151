// 2025/03/05 17:34:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/PassiveEntity.h"


struct Passive : public PassiveEntity
{
    Passive(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        PassiveEntity(name, researches, type, reader)
    {

    }
};
