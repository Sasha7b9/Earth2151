// 2022/09/24 17:53:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Lights.h"


Lights::Lights(FileInputStream &stream)
{
    for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
    {
        push_back(Light(stream));
    }
}
