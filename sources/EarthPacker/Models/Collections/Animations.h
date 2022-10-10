// 2022/09/24 21:03:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Models/Elements/Vector.h"
#include "Models/Elements/RotationFrame.h"


struct PositionOffsetFrames : public std::list<Vector>, public IInputStream
{
    void Create(bool unknown);

private:

    uint ReadUINT(pchar name);
};


struct RotationFrames : public std::list<RotationFrame>, public IInputStream
{
    void Create();

private:

    uint ReadUINT(pchar name);
};


class Animations : public IInputStream
{
public:
    void Create();

    PositionOffsetFrames unknownAnimationData;
    PositionOffsetFrames movementFrames;
    RotationFrames       rotationFrames;
};
