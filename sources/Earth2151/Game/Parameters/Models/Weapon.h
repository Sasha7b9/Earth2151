// 2025/03/05 14:27:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/InteractableEntity.h"


struct Weapon : public InteractableEntity
{
    Weapon(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        InteractableEntity(name, researhces, type, reader)
    {
        range_of_sight = reader.ReadInt();
        plug_type = reader.ReadInt();
        slot_type = reader.ReadInt();
        max_alpha_per_tick = reader.ReadInt();
        max_beta_per_tick = reader.ReadInt();
        alpha_margin = reader.ReadInt();
        beta_margin = reader.ReadInt();
        barrel_beta_type = reader.ReadInt();
        barrel_beta_angle = reader.ReadInt();
        barrel_count = reader.ReadInt();
        ammo_ID = reader.ReadString();

        reader.ReadUInt();
        ammo_type = reader.ReadInt();
        target_type = reader.ReadInt();
        range_of_fire = reader.ReadInt();
        plus_damage = reader.ReadInt();
        fire_type = reader.ReadInt();
        shoot_delay = reader.ReadInt();
        need_external = reader.ReadInt();
        reload_delay = reader.ReadInt();
        max_ammo = reader.ReadInt();
        Barrel_explosion_ID = reader.ReadString();

        reader.ReadUInt();
    }

    virtual void Save(int i, File &file) override
    {
        InteractableEntity::Save(i, file);

        file.WriteString(Text::Format("     range_of_sight %d, plug_type %d, slot_type %d, max_alpha_per_tick %d, max_beta_per_tick %d, alpha_margin %d,"
            "beta_margin %d, barrel_beta_type %d, barrel_beta_angle %d, barrel_count %d, ammo_ID \"%s\"",
            range_of_sight, plug_type, slot_type, max_alpha_per_tick, max_beta_per_tick, alpha_margin, beta_margin, barrel_beta_type, barrel_beta_angle, barrel_count, ammo_ID.c_str()));

        file.WriteString(Text::Format("     ammo_type %d, target_type %d, range_of_fire %d, plus_damage %d, fire_type %d, shoot_delay %d, need_external %d,"
            "reload_delay %d, max_ammo %d, Barrel_explosion_ID \"%s\"",
            ammo_type, target_type, range_of_fire, plus_damage, fire_type, shoot_delay, need_external, reload_delay, max_ammo, Barrel_explosion_ID.c_str()));
    }

    int range_of_sight;
    int plug_type;
    int slot_type;
    int max_alpha_per_tick;
    int max_beta_per_tick;
    int alpha_margin;
    int beta_margin;
    int barrel_beta_type;
    int barrel_beta_angle;
    int barrel_count;
    String<> ammo_ID;

    int ammo_type;
    int target_type;
    int range_of_fire;
    int plus_damage;
    int fire_type;
    int shoot_delay;
    int need_external;
    int reload_delay;
    int max_ammo;
    String<> Barrel_explosion_ID;
};
