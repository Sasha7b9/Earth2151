// 2025/03/05 19:59:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/DestructibleEntity.h"


struct BuilderLine : public DestructibleEntity
{
    BuilderLine(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {

    }
};
