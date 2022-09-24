// 2022/09/24 16:39:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


class Vector
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector(FileInputStream &);
};
