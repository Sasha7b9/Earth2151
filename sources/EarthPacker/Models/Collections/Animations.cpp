// 2022/09/24 21:03:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Animations.h"
#include "Models/Model.h"


void PositionOffsetFrames::Create(bool unknown)
{
    int length = 0;
    if (unknown)
    {
        length = ReadUINT("unkn frames");
    }
    else
    {
        length = ReadUINT("trans frames");
    }

    for (int i = 0; i < length; i++)
    {
        if (unknown)
        {
            push_back(Vector(wxString::Format("unkn a %d", i).c_str()));
        }
        else
        {
            push_back(Vector(wxString::Format("pos a %d", i).c_str()));
        }
    }
}


void RotationFrames::Create()
{
    int length = ReadUINT("rot frames");

    for (int i = 0; i < length; i++)
    {
        push_back(RotationFrame(i));
    }
}


void Animations::Create()
{
    unknownAnimationData.Create(true);
    movementFrames.Create(false);
    rotationFrames.Create();
}
