// 2022/09/24 16:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/MountPoints.h"


MountPoints::MountPoints(FileInputStream &stream)
{
    for (int i = 0; i < NUMBER_OF_MOUNTPOINTS; i++)
    {
        push_back(Vector(stream));
    }
}
