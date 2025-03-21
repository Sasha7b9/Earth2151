// 2025/03/05 12:54:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Vehicle.h"


struct Sapper : public Vehicle
{
    Sapper(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Vehicle(name, researhces, type, reader)
    {
        mines_look_range = reader.ReadInt();
        min_ID = reader.ReadString(4);
        reader.ReadUInt();
        max_mines_count = reader.ReadInt();

        anim_down.Read(reader);
        anim_up.Read(reader);

        put_min_smoke_ID = reader.ReadString(4);
        reader.ReadUInt();
    }

    int mines_look_range;
    String<> min_ID;
    int max_mines_count;

    AnimLine anim_down;
    AnimLine anim_up;

    String<> put_min_smoke_ID;
};
