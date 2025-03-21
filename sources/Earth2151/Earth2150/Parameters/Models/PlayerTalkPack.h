// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct PlayerTalkPack : public TypelessEntity
{
    PlayerTalkPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        base_under_attack = reader.ReadString(4);
        building_under_attack = reader.ReadString(4);
        space_port_under_attack = reader.ReadString(4);
        enemy_land_in_base = reader.ReadString(4);
        low_materials = reader.ReadString(4);
        low_materials_in_base = reader.ReadString(4);
        low_power = reader.ReadString(4);
        low_power_in_base = reader.ReadString(4);
        research_complete = reader.ReadString(4);
        production_started = reader.ReadString(4);
        production_completed = reader.ReadString(4);
        production_cancelled = reader.ReadString(4);
        platoon_lost = reader.ReadString(4);
        platoon_created = reader.ReadString(4);
        platoon_disbanded = reader.ReadString(4);
        unit_lost = reader.ReadString(4);
        transporter_arrived = reader.ReadString(4);
        artefact_located = reader.ReadString(4);
        artefact_recovered = reader.ReadString(4);
        new_area_location_found = reader.ReadString(4);
        enemy_main_base_located = reader.ReadString(4);
        new_source_field_located = reader.ReadString(4);
        source_field_exploited = reader.ReadString(4);
        building_lost = reader.ReadString(4);
    }

    String<> base_under_attack;
    String<> building_under_attack;
    String<> space_port_under_attack;
    String<> enemy_land_in_base;
    String<> low_materials;
    String<> low_materials_in_base;
    String<> low_power;
    String<> low_power_in_base;
    String<> research_complete;
    String<> production_started;
    String<> production_completed;
    String<> production_cancelled;
    String<> platoon_lost;
    String<> platoon_created;
    String<> platoon_disbanded;
    String<> unit_lost;
    String<> transporter_arrived;
    String<> artefact_located;
    String<> artefact_recovered;
    String<> new_area_location_found;
    String<> enemy_main_base_located;
    String<> new_source_field_located;
    String<> source_field_exploited;
    String<> building_lost;
};
