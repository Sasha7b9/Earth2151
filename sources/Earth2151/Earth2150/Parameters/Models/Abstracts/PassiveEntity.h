// 2025/03/05 17:31:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/DestructibleEntity.h"


struct PassiveEntity : public DestructibleEntity
{
    PassiveEntity(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        passive_mask = reader.ReadInt();
        wall_copula_ID = reader.ReadString(4);

        reader.ReadUInt();
    }


    virtual void Save(int i, File &file) override
    {
        DestructibleEntity::Save(i, file);

        file.WriteString(Text::Format("     mask : %d, wall copula ID : %s,", passive_mask, wall_copula_ID.c_str()));
    }

    int passive_mask;
    String<> wall_copula_ID;
};
