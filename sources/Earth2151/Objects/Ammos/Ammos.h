// 2025/02/02 10:33:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Objects.h"


class AmmoObject : public MovedObject
{
public:

    AmmoObject(pchar name_type) : MovedObject(name_type)
    {
        MovedObject::OnEventRebuild();
    }
};
