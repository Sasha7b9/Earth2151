// 2022/09/24 21:09:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/RotationFrame.h"


RotationFrame::RotationFrame(FileInputStream &stream)
{
    transform[0][0] = stream.ReadFloat();
    transform[0][1] = stream.ReadFloat();
    transform[0][2] = stream.ReadFloat();
    transform[0][3] = stream.ReadFloat();
    transform[1][0] = stream.ReadFloat();
    transform[1][1] = stream.ReadFloat();
    transform[1][2] = stream.ReadFloat();
    transform[1][3] = stream.ReadFloat();
    transform[2][0] = stream.ReadFloat();
    transform[2][1] = stream.ReadFloat();
    transform[2][2] = stream.ReadFloat();
    transform[2][3] = stream.ReadFloat();
    transform[3][0] = stream.ReadFloat();
    transform[3][1] = stream.ReadFloat();
    transform[3][2] = stream.ReadFloat();
    transform[3][3] = stream.ReadFloat();
}
