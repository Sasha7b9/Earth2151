// 2022/09/24 16:39:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Vector.h"


Vector::Vector(FileInputStream &stream)
{
    Create(stream);
}


void Vector::Create(FileInputStream &stream)
{
    x = stream.ReadFloat();

    y = -stream.ReadFloat();

    z = stream.ReadFloat();
}
