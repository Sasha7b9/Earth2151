// 2025/03/05 11:07:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/DestructibleEntity.h"


struct EquipableEntity : public DestructibleEntity
{
    EquipableEntity(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        sight_range = reader.ReadInt();
        talk_pack_ID = reader.ReadString(4);
        unknown = reader.ReadInt();
        shield_generator_ID = reader.ReadString(4);
        unknown2 = reader.ReadInt();
        max_shield_update = reader.ReadInt();
        type_slot1 = reader.ReadInt();
        type_slot2 = reader.ReadInt();
        type_slot3 = reader.ReadInt();
        type_slot4 = reader.ReadInt();
    }

    virtual void Save(int i, File &file) override
    {
        DestructibleEntity::Save(i, file);

        file.WriteString(Text::Format("     sight_range %d, talk_pack_ID \"%s\", shield_generator_ID \"%s\", max_shield_update %d, slots : %d %d %d %d",
            sight_range, talk_pack_ID.c_str(), shield_generator_ID.c_str(), max_shield_update, type_slot1, type_slot2, type_slot3, type_slot4));
    }

    int sight_range;
    String<> talk_pack_ID;
    int unknown;
    String<> shield_generator_ID;
    int unknown2;
    int max_shield_update;
    int type_slot1;
    int type_slot2;
    int type_slot3;
    int type_slot4;
};
