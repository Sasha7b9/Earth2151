// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct Parameter : public TypelessEntity
{
    Parameter(const String<> &name, const Array<int> &researches, FileReader &reader, const Array<bool> &_field_types) :
        TypelessEntity(name, researches)
    {
        for (int i = 0; i < _field_types.GetArrayElementCount(); i++)
        {
            field_types.AppendArrayElement(_field_types[i]);
        }

        for (int i = 0; i < field_types.GetArrayElementCount(); i++)
        {
            if (field_types[i])
            {
                values.AppendArrayElement(reader.ReadString(4));
            }
            else
            {
                int int_value = reader.ReadInt();
                values.AppendArrayElement(Text::Format("%d", int_value));
            }
        }
    }

    virtual ~Parameter()
    {
        values.PurgeArray();
        field_types.PurgeArray();
    }

    Array<bool> field_types;

    Array<String<>> values;
};
