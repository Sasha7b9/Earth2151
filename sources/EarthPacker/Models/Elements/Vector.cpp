// 2022/09/24 16:39:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Vector.h"
#include "Models/Model.h"


Vector::Vector(uint8 *bytes)
{
    memcpy(&x, bytes, 4);
    memcpy(&y, bytes + 4, 4);
    memcpy(&z, bytes + 8, 4);
}


Vector::Vector(DescriptionModel &desc, pchar name)
{
    Create(desc, name);
}


void Vector::Create(DescriptionModel &desc, pchar name)
{
    static int counter = 0;

    InfoModel info(InfoModel::Type::Vector, stream->TellI(), name);

    x = stream->ReadFloat();

    y = -stream->ReadFloat();

    z = stream->ReadFloat();

    info.AppendBytes(x);
    info.AppendBytes(y);
    info.AppendBytes(z);

    desc.AppendInfo(info);
}


string Vector::ToString() const
{
    return wxString::Format("(%f , %f , %f)", x, y, z).c_str().AsChar();
}
