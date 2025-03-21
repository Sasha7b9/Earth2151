// 2025/03/05 16:34:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/Entity.h"


struct TypelessEntity : public Entity
{
    TypelessEntity(const String<> &name, const Array<int> &researches) 
        : Entity(name, researches, EntityClassType::None)
    {

    }

    virtual ~TypelessEntity()
    {

    }
};
