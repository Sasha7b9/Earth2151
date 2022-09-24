// 2022/09/24 19:46:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vector.h"


class Vertex
{
public:
    Vertex(const Vector &position, const Vector &normal, float u, float v);

    Vector position;
    Vector normal;
    float u;
    float v;
};
