// 2022/09/24 17:55:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vector.h"
#include "Utils/FileInputStream.h"


struct InfoModel;


struct Light : public Vector
{
    static const int COUNT = 5;

    Light(FileInputStream &);

    static void ToInfo(InfoModel &);

    wxColour color;

    float length;
    float direction;
    float width;
    float u3;
    float tilt;
    float ambience;
};
