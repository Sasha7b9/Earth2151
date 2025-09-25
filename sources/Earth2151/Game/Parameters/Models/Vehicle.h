// 2025/03/05 11:05:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/EquipableEntity.h"


struct Vehicle : public EquipableEntity
{
    Vehicle(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        EquipableEntity(name, researhces, type, reader)
    {
        soil_speed = reader.ReadInt();
        road_speed = reader.ReadInt();
        sand_speed = reader.ReadInt();
        bank_speed = reader.ReadInt();
        water_speed = reader.ReadInt();
        deep_water_speed = reader.ReadInt();
        air_speed = reader.ReadInt();
        object_type = reader.ReadInt();
        engine_smoke_ID = reader.ReadString();
        reader.ReadUInt();
        dist_ID = reader.ReadString();
        reader.ReadUInt();
        billow_ID = reader.ReadString();
        reader.ReadUInt();
        stand_billow_ID = reader.ReadString();
        reader.ReadUInt();
        track_ID = reader.ReadString();
        reader.ReadUInt();
    }

    virtual void Save(int i, File &file) override
    {
        EquipableEntity::Save(i, file);

        file.WriteString(Text::Format("     soil_speed %d, road_speed %d, sand_speed %d, bank_speed %d, water_speed %d, deep_water_speed %d, air_speed %d, object_type %d",
            soil_speed, road_speed, sand_speed, bank_speed, water_speed, deep_water_speed, air_speed, object_type));

        file.WriteString(Text::Format("     engine_smoke_ID \"%s\", dist_ID \"%s\", billow_ID \"%s\", stand_billow_ID \"%s\", track_ID \"%s\"",
            engine_smoke_ID.c_str(), dist_ID.c_str(), billow_ID.c_str(), stand_billow_ID.c_str(), track_ID.c_str()));
    }

    int soil_speed;
    int road_speed;
    int sand_speed;
    int bank_speed;
    int water_speed;
    int deep_water_speed;
    int air_speed;
    int object_type;
    String<> engine_smoke_ID;
    String<> dist_ID;
    String<> billow_ID;
    String<> stand_billow_ID;
    String<> track_ID;
};
