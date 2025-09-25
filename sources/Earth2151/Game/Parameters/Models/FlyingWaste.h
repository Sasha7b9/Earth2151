// 2025/03/05 19:38:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/DestructibleEntity.h"


struct FlyingWaste : public DestructibleEntity
{
    FlyingWaste(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        DestructibleEntity(name, researhces, type, reader)
    {
        waster_size = reader.ReadInt();

        for (int i = 0; i < 4; i++)
        {
            param[i].Read(reader);
        }

        flight_time = reader.ReadInt();
        waste_speed = reader.ReadInt();
        waste_distance_X4 = reader.ReadInt();
        waste_beta = reader.ReadInt();
    }

    int waster_size;

    struct Parameters
    {
        String<> sub_waste_ID;
        uint unknown;
        int sub_waste_alpha;

        void Read(FileReader &);
    };

    Parameters param[4];

    int flight_time;
    int waste_speed;
    int waste_distance_X4;
    int waste_beta;
};


void FlyingWaste::Parameters::Read(FileReader &reader)
{
    sub_waste_ID = reader.ReadString();
    unknown = reader.ReadUInt();
    sub_waste_alpha = reader.ReadInt();
}
