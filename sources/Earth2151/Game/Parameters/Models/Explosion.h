// 2025/03/05 19:28:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct Explosion : public DestructibleEntity
{
    Explosion(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        ticks = reader.ReadInt();
        flags = reader.ReadInt();
    }

    int ticks;
    int flags;
};
