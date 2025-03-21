// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct SpecialUpdateLink : public TypelessEntity
{
    SpecialUpdateLink(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        value = reader.ReadString(4);
        unknown = reader.ReadUInt();
    }

    String<> value;
    uint unknown;
};
