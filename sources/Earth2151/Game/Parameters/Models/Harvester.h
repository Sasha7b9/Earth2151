// 2025/03/05 12:38:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Harvester : public Vehicle
{
    Harvester(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Vehicle(name, researhces, type, reader)
    {
        container_coun = reader.ReadInt();
        ticks_per_container = reader.ReadInt();
        put_resource_angle = reader.ReadInt();

        anim_start.Read(reader);
        anim_work.Read(reader);
        anim_end.Read(reader);

        harvest_smoke_ID = reader.ReadString();
        null_uint = reader.ReadUInt();
    }

    int container_coun;
    int ticks_per_container;
    int put_resource_angle;

    AnimLine anim_start;
    AnimLine anim_work;
    AnimLine anim_end;

    uint null_uint;

    String<> harvest_smoke_ID;
};
