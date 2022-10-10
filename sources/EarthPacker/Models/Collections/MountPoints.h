// 2022/09/24 16:37:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vector.h"
#include "Utils/FileInputStream.h"


class MountPoints : public std::list<Vector>, public IInputStream
{
public:
    MountPoints();
private:
    static const int NUMBER_OF_MOUNTPOINTS = 4;
};
