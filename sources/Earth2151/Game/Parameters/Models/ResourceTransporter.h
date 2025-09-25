// 2025/03/05 13:21:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/VerticalTransporter.h"


struct ResourceTransporter : public VerticalTransporter
{
    ResourceTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        VerticalTransporter(name, researhces, type, reader)
    {
        resource_vehicle_type = reader.ReadInt();
        animated_transporter_stop = reader.ReadInt();
        show_video_per_transporters_count = reader.ReadInt();
        total_orbital_money = reader.ReadInt();
    }

    int resource_vehicle_type;
    int animated_transporter_stop;
    int show_video_per_transporters_count;
    int total_orbital_money;
};
