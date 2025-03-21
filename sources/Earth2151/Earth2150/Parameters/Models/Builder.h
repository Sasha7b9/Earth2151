// 2025/03/05 10:58:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Earth2150/Parameters/Models/Vehicle.h"


struct Builder : public Vehicle
{
    Builder(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Vehicle(name, researhces, type, reader)
    {
        wall_ID = reader.ReadString(4);
        reader.ReadUInt();
        bridge_ID = reader.ReadString(4);
        reader.ReadUInt();
        tunnel_number = reader.ReadInt();
        road_build_time = reader.ReadInt();
        flat_build_teim = reader.ReadInt();
        trench_build_time = reader.ReadInt();
        tunnel_build_time = reader.ReadInt();
        build_object_animation_angle = reader.ReadInt();
        dig_normal_animatioini_angle = reader.ReadInt();
        dig_low_animatioini_angle = reader.ReadInt();

        anim_object_start.Read(reader);
        anim_object_work.Read(reader);
        anim_object_end.Read(reader);

        anim_dig_normal_start.Read(reader);
        anim_dig_normal_work.Read(reader);
        anim_dig_normal_end.Read(reader);

        anim_dig_low_start.Read(reader);
        anim_dig_low_work.Read(reader);
        anim_dig_low_end.Read(reader);

        dig_smoke_ID = reader.ReadString(4);
        reader.ReadUInt();
    }

    String<> wall_ID;
    String<> bridge_ID;
    int tunnel_number;
    int road_build_time;
    int flat_build_teim;
    int trench_build_time;
    int tunnel_build_time;
    int build_object_animation_angle;
    int dig_normal_animatioini_angle;
    int dig_low_animatioini_angle;

    AnimLine anim_object_start;
    AnimLine anim_object_work;
    AnimLine anim_object_end;

    AnimLine anim_dig_normal_start;
    AnimLine anim_dig_normal_work;
    AnimLine anim_dig_normal_end;

    AnimLine anim_dig_low_start;
    AnimLine anim_dig_low_work;
    AnimLine anim_dig_low_end;

    String<> dig_smoke_ID;
};
