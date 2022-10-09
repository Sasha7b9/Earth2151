// 2022/09/24 21:09:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/RotationFrame.h"


RotationFrame::RotationFrame()
{
    (*this)[0][0] = FileInputStream::Get()->ReadFloat();
    (*this)[0][1] = FileInputStream::Get()->ReadFloat();
    (*this)[0][2] = FileInputStream::Get()->ReadFloat();
    (*this)[0][3] = FileInputStream::Get()->ReadFloat();
    (*this)[1][0] = FileInputStream::Get()->ReadFloat();
    (*this)[1][1] = FileInputStream::Get()->ReadFloat();
    (*this)[1][2] = FileInputStream::Get()->ReadFloat();
    (*this)[1][3] = FileInputStream::Get()->ReadFloat();
    (*this)[2][0] = FileInputStream::Get()->ReadFloat();
    (*this)[2][1] = FileInputStream::Get()->ReadFloat();
    (*this)[2][2] = FileInputStream::Get()->ReadFloat();
    (*this)[2][3] = FileInputStream::Get()->ReadFloat();
    (*this)[3][0] = FileInputStream::Get()->ReadFloat();
    (*this)[3][1] = FileInputStream::Get()->ReadFloat();
    (*this)[3][2] = FileInputStream::Get()->ReadFloat();
    (*this)[3][3] = FileInputStream::Get()->ReadFloat();
}
