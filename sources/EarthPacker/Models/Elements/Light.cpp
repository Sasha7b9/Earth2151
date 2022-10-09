// 2022/09/24 17:55:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Light.h"
#include "Models/Model.h"


Light::Light(FileInputStream &stream, DescriptionModel &desc, const wxString &name)
{
    InfoModel info(InfoModel::Type::Light, stream.TellI(), name.c_str());

    position = Vector(stream, desc, "position");
    color = CreateColor();
    length = stream.ReadFloat();
    direction = stream.ReadFloat();
    width = stream.ReadFloat();
    u3 = stream.ReadFloat();
    tilt = stream.ReadFloat();
    ambience = stream.ReadFloat();

    desc.AppendInfo(info);
}


wxColour Light::CreateColor()
{
    FileInputStream &stream = *FileInputStream::Get();
    DescriptionModel &desc = *DescriptionModel::Get();

    InfoModel info(InfoModel::Type::Color, stream.TellI(), "color");

    float r = stream.ReadFloat();
    info.AppendBytes(r);
    r *= 255;

    float g = stream.ReadFloat();
    info.AppendBytes(g);
    g *= 255;

    float b = stream.ReadFloat();
    info.AppendBytes(b);
    b *= 255;

    desc.AppendInfo(info);

    return wxColour((uint8)r, (uint8)g, (uint8)b);
}
