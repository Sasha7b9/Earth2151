// 2025/03/05 14:40:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct Missile : public DestructibleEntity
{
    Missile(const String<> &name, const Array<int> &researhces, EntityClassType::E _type, FileReader &reader) :
        DestructibleEntity(name, researhces, _type, reader)
    {
        missile_type = reader.ReadInt();
        rocket_type = reader.ReadInt();
        missile_size = reader.ReadInt();
        rocket_dummy_ID = reader.ReadString();

        reader.ReadUInt();
        is_anti_rocket_target = reader.ReadInt();
        speed = reader.ReadInt();
        time_of_shoot = reader.ReadInt();
        plus_range_of_fire = reader.ReadInt();
        hit_type = reader.ReadInt();
        hit_range = reader.ReadInt();
        type_of_damage = reader.ReadInt();
        damage = reader.ReadInt();
        explosion_ID = reader.ReadString();

        reader.ReadUInt();
    }

    int missile_type;
    int rocket_type;
    int missile_size;
    String<> rocket_dummy_ID;
    int is_anti_rocket_target;
    int speed;
    int time_of_shoot;
    int plus_range_of_fire;
    int hit_type;
    int hit_range;
    int type_of_damage;
    int damage;
    String<> explosion_ID;
};
