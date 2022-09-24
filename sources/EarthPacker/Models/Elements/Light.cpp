// 2022/09/24 17:55:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Light.h"


Light::Light(FileInputStream &stream) : Vector(stream)
{
    float r = stream.ReadFloat() * 255;
    float g = stream.ReadFloat() * 255;
    float b = stream.ReadFloat() * 255;

    color.Set((uint8)r, (uint8)g, (uint8)b);

    length = stream.ReadFloat();
    direction = stream.ReadFloat();
    width = stream.ReadFloat();
    u3 = stream.ReadFloat();
    tilt = stream.ReadFloat();
    ambience = stream.ReadFloat();
}
