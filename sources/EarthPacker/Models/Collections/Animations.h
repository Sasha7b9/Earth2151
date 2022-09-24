// 2022/09/24 21:03:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Models/Elements/Vector.h"


struct PositionOffsetFrames : public std::list<Vector>
{
    void Create(FileInputStream &);
};


struct RotationFrames : public std::list<


class Animations
{
public:
    void Create(FileInputStream &);
};
