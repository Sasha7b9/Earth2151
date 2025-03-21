// 2025/03/05 14:00:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/EquipableEntity.h"


struct Building : public EquipableEntity
{
    Building(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        EquipableEntity(name, researhces, type, reader)
    {
        building_type = reader.ReadInt();
        building_type_ex = reader.ReadInt();
        building_tab_type = reader.ReadInt();
        init_cannon_ID1 = reader.ReadString(4);

        reader.ReadUInt();
        init_cannon_ID2 = reader.ReadString(4);

        reader.ReadUInt();
        init_cannon_ID3 = reader.ReadString(4);

        reader.ReadUInt();
        init_cannon_ID4 = reader.ReadString(4);

        reader.ReadUInt();
        copula_ID = reader.ReadString(4);

        reader.ReadUInt();
        building_tunnel_number = reader.ReadInt();
        upgrade_copula_small_ID = reader.ReadString(4);

        reader.ReadUInt();
        upgrade_copula_big_ID = reader.ReadString(4);

        reader.ReadUInt();
        build_LC_transporter_ID = reader.ReadString(4);

        reader.ReadUInt();
        chimmey_smoke_ID = reader.ReadString(4);

        reader.ReadUInt();
        need_power = reader.ReadInt();
        slave_building_ID = reader.ReadString(4);

        reader.ReadUInt();
        max_sub_building_count = reader.ReadInt();
        power_level = reader.ReadInt();
        power_transmitter_range = reader.ReadInt();
        connect_transmitter_range = reader.ReadInt();
        full_energy_power_in_day = reader.ReadInt();
        resource_input_output = reader.ReadInt();
        tick_per_container = reader.ReadInt();
        container_ID = reader.ReadString(4);

        reader.ReadUInt();
        container_smelting_ticks = reader.ReadInt();
        resources_per_transport = reader.ReadInt();
        transporter_ID = reader.ReadString(4);

        reader.ReadUInt();
        building_ammo_ID = reader.ReadString(4);

        reader.ReadUInt();
        range_of_building_fire = reader.ReadInt();
        shoot_explosion_ID = reader.ReadString(4);

        reader.ReadUInt();
        ammo_reloading_time = reader.ReadInt();
        build_explosion_ID = reader.ReadString(4);

        reader.ReadUInt();
        copula_animation_flags = reader.ReadInt();
        end_of_closing_copula_animation = reader.ReadInt();
        laser_ID = reader.ReadString(4);

        reader.ReadUInt();
        space_station_type = reader.ReadInt();
    }

    int building_type;
    int building_type_ex;
    int building_tab_type;
    String<> init_cannon_ID1;
    String<> init_cannon_ID2;
    String<> init_cannon_ID3;
    String<> init_cannon_ID4;
    String<> copula_ID;
    int building_tunnel_number;
    String<> upgrade_copula_small_ID;
    String<> upgrade_copula_big_ID;
    String<> build_LC_transporter_ID;
    String<> chimmey_smoke_ID;
    int need_power;
    String<> slave_building_ID;
    int max_sub_building_count;
    int power_level;
    int power_transmitter_range;
    int connect_transmitter_range;
    int full_energy_power_in_day;
    int resource_input_output;
    int tick_per_container;
    String<> container_ID;
    int container_smelting_ticks;
    int resources_per_transport;
    String<> transporter_ID;
    String<> building_ammo_ID;
    int range_of_building_fire;
    String<> shoot_explosion_ID;
    int ammo_reloading_time;
    String<> build_explosion_ID;
    int copula_animation_flags;
    int end_of_closing_copula_animation;
    String<> laser_ID;
    int space_station_type;
};
