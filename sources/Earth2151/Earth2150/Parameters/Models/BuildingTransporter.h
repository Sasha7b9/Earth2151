// 2025/03/05 13:14:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/VerticalTransporter.h"


struct BuildingTransporter : public VerticalTransporter
{
    BuildingTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        VerticalTransporter(name, researhces, type, reader)
    {
        builder_line_ID = reader.ReadString(4);
        unknown3 = reader.ReadInt();
    }

    String<> builder_line_ID;
    int unknown3;
};
