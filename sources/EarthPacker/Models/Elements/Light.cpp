// 2022/09/24 17:55:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Light.h"
#include "Models/Model.h"


Light::Light(FileInputStream &stream, DescriptionModel &desc, const wxString &name)
{
    InfoModel info(stream.TellI(), name.c_str());

    position = Vector(stream);
    info.AppendBytes(position);

    float r = stream.ReadFloat();
    info.AppendBytes(r);
    r *= 255;

    float g = stream.ReadFloat();
    info.AppendBytes(g);
    g *= 255;

    float b = stream.ReadFloat();
    info.AppendBytes(b);
    b *= 255;

    color.Set((uint8)r, (uint8)g, (uint8)b);

    length = stream.ReadFloat();
    info.AppendBytes(length);

    direction = stream.ReadFloat();
    info.AppendBytes(direction);

    width = stream.ReadFloat();
    info.AppendBytes(width);

    u3 = stream.ReadFloat();
    info.AppendBytes(u3);

    tilt = stream.ReadFloat();
    info.AppendBytes(tilt);

    ambience = stream.ReadFloat();
    info.AppendBytes(ambience);

    desc.AppendInfo(info, stream);
}
