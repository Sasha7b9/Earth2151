// 2022/09/24 21:03:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Animations.h"


void PositionOffsetFrames::Create(FileInputStream &stream)
{
    int length = stream.ReadINT();

    for (int i = 0; i < length; i++)
    {
        push_back(Vector(stream));
    }
}


void Animations::Create(FileInputStream &stream)
{

}
