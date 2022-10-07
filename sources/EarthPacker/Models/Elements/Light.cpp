// 2022/09/24 17:55:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Light.h"
#include "Models/Model.h"


Light::Light(FileInputStream &stream, DescriptionModel &desc, const wxString &name)
{
    InfoModel info(stream.TellI(), 0, name.c_str());

    position = Vector(stream);
    info.Append(position);

    float r = stream.ReadFloat();
    info.Append(r);
    r *= 255;

    float g = stream.ReadFloat();
    info.Append(g);
    g *= 255;

    float b = stream.ReadFloat();
    info.Append(b);
    b *= 255;

    color.Set((uint8)r, (uint8)g, (uint8)b);

    length = stream.ReadFloat();
    info.Append(length);

    direction = stream.ReadFloat();
    info.Append(direction);

    width = stream.ReadFloat();
    info.Append(width);

    u3 = stream.ReadFloat();
    info.Append(u3);

    tilt = stream.ReadFloat();
    info.Append(tilt);

    ambience = stream.ReadFloat();
    info.Append(ambience);

    info.size = (int)stream.TellI() - info.address;

    desc.AppendInfo(info);
}
