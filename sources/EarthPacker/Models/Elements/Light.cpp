// 2022/09/24 17:55:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Light.h"
#include "Models/Model.h"


Light::Light(DescriptionModel &desc, const wxString &name)
{
    InfoModel info(InfoModel::Type::Light, stream->TellI(), name.c_str());

    position = Vector(desc, "position");
    color = CreateColor();
    length = ReadFloat("length");
    direction = ReadFloat("direction");
    width = ReadFloat("width");
    u3 = ReadFloat("u3");
    tilt = ReadFloat("tilt");
    ambience = ReadFloat("ambience");

    info.size = (int)stream->TellI() - info.header.offset;

    desc.AppendInfo(info);
}


wxColour Light::CreateColor()
{
    DescriptionModel &desc = *DescriptionModel::Get();

    InfoModel info(InfoModel::Type::Color, stream->TellI(), "color");

    float r = stream->ReadFloat();
    info.AppendBytes(r);
    r *= 255;

    float g = stream->ReadFloat();
    info.AppendBytes(g);
    g *= 255;

    float b = stream->ReadFloat();
    info.AppendBytes(b);
    b *= 255;

    desc.AppendInfo(info);

    return wxColour((uint8)r, (uint8)g, (uint8)b);
}


float Light::ReadFloat(pchar name)
{
    DescriptionModel &desc = *DescriptionModel::Get();

    InfoModel info(InfoModel::Type::Float, stream->TellI(), name);

    float result = stream->ReadFloat();

    desc.AppendInfo(info.AppendBytes(result));

    return result;
}
