// 2025/03/05 13:25:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/VerticalTransporter.h"


struct UnitTransporter : public VerticalTransporter
{
    UnitTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        VerticalTransporter(name, researhces, type, reader)
    {
        units_count = reader.ReadInt();
        docking_height = reader.ReadInt();

        anim_load_start.Read(reader);
        anim_load_end.Read(reader);
        anim_unload_start.Read(reader);
        anim_unload_end.Read(reader);
    }

    int units_count = 0;
    int docking_height = 0;

    AnimLine anim_load_start;
    AnimLine anim_load_end;
    AnimLine anim_unload_start;
    AnimLine anim_unload_end;
};
