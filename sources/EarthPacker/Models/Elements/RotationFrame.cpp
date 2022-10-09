// 2022/09/24 21:09:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/RotationFrame.h"


RotationFrame::RotationFrame(FileInputStream &stream)
{
    (*this)[0][0] = stream.ReadFloat();
    (*this)[0][1] = stream.ReadFloat();
    (*this)[0][2] = stream.ReadFloat();
    (*this)[0][3] = stream.ReadFloat();
    (*this)[1][0] = stream.ReadFloat();
    (*this)[1][1] = stream.ReadFloat();
    (*this)[1][2] = stream.ReadFloat();
    (*this)[1][3] = stream.ReadFloat();
    (*this)[2][0] = stream.ReadFloat();
    (*this)[2][1] = stream.ReadFloat();
    (*this)[2][2] = stream.ReadFloat();
    (*this)[2][3] = stream.ReadFloat();
    (*this)[3][0] = stream.ReadFloat();
    (*this)[3][1] = stream.ReadFloat();
    (*this)[3][2] = stream.ReadFloat();
    (*this)[3][3] = stream.ReadFloat();
}
