// 2025/03/05 11:10:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/Entity.h"


struct TypedEntity : public Entity
{
    TypedEntity(const String<> &_name, const Array<int> &_ressearches, EntityClassType::E _type) :
        Entity(_name, _ressearches, _type)
    {

    }
};
