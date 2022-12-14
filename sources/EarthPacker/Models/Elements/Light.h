// 2022/09/24 17:55:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vector.h"
#include "Utils/FileInputStream.h"


struct Light : public IInputStream
{
    static const int COUNT = 5;

    Light(const wxString &name);

    wxColour color;
    Vector position;
    float length;
    float direction;
    float width;
    float u3;
    float tilt;
    float ambience;

private:

    wxColour CreateColor();
    float ReadFloat(pchar name);
};
