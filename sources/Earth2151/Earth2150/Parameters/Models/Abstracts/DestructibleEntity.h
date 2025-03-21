// 2025/03/05 11:08:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/InteractableEntity.h"


struct DestructibleEntity : public InteractableEntity
{
    DestructibleEntity(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        InteractableEntity(name, researhces, type, reader)
    {
        HP = reader.ReadInt();
        regenerationHP = reader.ReadInt();
        armor = reader.ReadInt();
        calorific_capacity = reader.ReadInt();
        disable_resist = reader.ReadInt();
        storeable_flags = reader.ReadInt();
        stand_type = reader.ReadInt();
    }

    virtual void Save(int i, File &file) override
    {
        InteractableEntity::Save(i, file);

        file.WriteString(Text::Format("     HP %d, regeneration HP %d, armor %d, calorific capacity %d, disable resist %d, storaable flags %d, stand type %d",
            HP, regenerationHP, armor, calorific_capacity, disable_resist, storeable_flags, stand_type));
    }

    int HP;
    int regenerationHP;
    int armor;
    int calorific_capacity;
    int disable_resist;
    int storeable_flags;
    int stand_type;
};
