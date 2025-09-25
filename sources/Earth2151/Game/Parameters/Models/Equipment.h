// 2025/03/05 16:43:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/InteractableEntity.h"


struct Equipment : public InteractableEntity
{
    Equipment(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        InteractableEntity(name, researhces, type, reader)
    {
        range_of_sight = reader.ReadInt();
        plug_type = reader.ReadInt();
        slot_type = reader.ReadInt();
        max_alpha_per_tick = reader.ReadInt();
        max_beta_per_tick = reader.ReadInt();
    }

    int range_of_sight;
    int plug_type;
    int slot_type;
    int max_alpha_per_tick;
    int max_beta_per_tick;
};
