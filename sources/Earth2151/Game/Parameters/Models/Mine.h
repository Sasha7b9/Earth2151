// 2025/03/05 19:45:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct Mine : public DestructibleEntity
{
    Mine(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        mine_size = reader.ReadInt();
        mine_type_damage = reader.ReadInt();
        mine_damage = reader.ReadInt();
    }

    int mine_size;
    int mine_type_damage;
    int mine_damage;
};

