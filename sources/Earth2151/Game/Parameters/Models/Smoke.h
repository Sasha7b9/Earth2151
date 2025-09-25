// 2025/03/05 19:33:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct Smoke : public DestructibleEntity
{
    Smoke(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        mesh1 = reader.ReadString();
        mesh2 = reader.ReadString();
        mesh3 = reader.ReadString();

        reader.ReadBuffer(&param, sizeof(param));
    }

    String<> mesh1;
    String<> mesh2;
    String<> mesh3;

    struct Parameters
    {
        int smoke_time1;
        int smoke_time2;
        int smoke_time3;
        int smoke_frequency;
        int starting_time;
        int smoking_time;
        int ending_time;
        int smoke_up_speed;
        int new_smoke_distance;
    };

    Parameters param;
};
