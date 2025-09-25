// 2025/03/05 19:56:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct WallLaser : public DestructibleEntity
{
    WallLaser(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {

    }
};
