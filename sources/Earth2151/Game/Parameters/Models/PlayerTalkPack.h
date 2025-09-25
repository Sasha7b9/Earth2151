// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/TypelessEntity.h"


struct PlayerTalkPack : public TypelessEntity
{
    PlayerTalkPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        base_under_attack = reader.ReadString();
        building_under_attack = reader.ReadString();
        space_port_under_attack = reader.ReadString();
        enemy_land_in_base = reader.ReadString();
        low_materials = reader.ReadString();
        low_materials_in_base = reader.ReadString();
        low_power = reader.ReadString();
        low_power_in_base = reader.ReadString();
        research_complete = reader.ReadString();
        production_started = reader.ReadString();
        production_completed = reader.ReadString();
        production_cancelled = reader.ReadString();
        platoon_lost = reader.ReadString();
        platoon_created = reader.ReadString();
        platoon_disbanded = reader.ReadString();
        unit_lost = reader.ReadString();
        transporter_arrived = reader.ReadString();
        artefact_located = reader.ReadString();
        artefact_recovered = reader.ReadString();
        new_area_location_found = reader.ReadString();
        enemy_main_base_located = reader.ReadString();
        new_source_field_located = reader.ReadString();
        source_field_exploited = reader.ReadString();
        building_lost = reader.ReadString();
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
