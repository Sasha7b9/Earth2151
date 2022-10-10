// 2022/09/24 20:55:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Faces.h"


void Faces::Create(DescriptionModel &desc)
{
    int faces = (int)ReadUINT("num faces");

    for (int i = 0; i < faces; i++)
    {
        push_back(Face(desc, i));
    }
}
