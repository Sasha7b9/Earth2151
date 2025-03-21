// 2025/03/05 17:49:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/InteractableEntity.h"


struct MultiExplosion : public InteractableEntity
{
    MultiExplosion(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        InteractableEntity(name, researhces, type, reader)
    {
        use_down_building = reader.ReadInt();
        down_building_start = reader.ReadInt();
        down_building_time = reader.ReadInt();

        for (int i = 0; i < 8; i++)
        {
            structs[i].Read(reader);
        }
    }

    int use_down_building;
    int down_building_start;
    int down_building_time;

    struct StructExp
    {
        String<> sub_object;
        uint unknown;
        int time;
        int angle;
        int dist4X;

        void Read(FileReader &);
    };

    StructExp structs[8];
};


void MultiExplosion::StructExp::Read(FileReader &reader)
{
    sub_object = reader.ReadString(4);
    unknown = reader.ReadUInt();
    time = reader.ReadInt();
    angle = reader.ReadInt();
    dist4X = reader.ReadInt();
}
