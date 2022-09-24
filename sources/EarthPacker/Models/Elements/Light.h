// 2022/09/24 17:55:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vector.h"
#include "Utils/FileInputStream.h"


class MLight : public Vector
{
public:
    MLight(FileInputStream &);

    wxColour color;

    float length;
}
