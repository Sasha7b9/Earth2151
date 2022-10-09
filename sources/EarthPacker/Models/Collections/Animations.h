// 2022/09/24 21:03:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Models/Elements/Vector.h"
#include "Models/Elements/RotationFrame.h"


struct DescriptionModel;


struct PositionOffsetFrames : public std::list<Vector>
{
    void Create(FileInputStream &, DescriptionModel &, bool unknown);

private:

    uint ReadUINT(pchar name);
};


struct RotationFrames : public std::list<RotationFrame>
{
    void Create();

private:

    uint ReadUINT(pchar name);
};


class Animations
{
public:
    void Create(FileInputStream &, DescriptionModel &);

    PositionOffsetFrames unknownAnimationData;
    PositionOffsetFrames movementFrames;
    RotationFrames       rotationFrames;
};
