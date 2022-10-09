// 2022/09/24 16:39:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Vector.h"
#include "Models/Model.h"


Vector::Vector(FileInputStream &stream, DescriptionModel &desc)
{
    Create(stream, desc);
}


void Vector::Create(FileInputStream &stream, DescriptionModel &desc)
{
    static int counter = 0;

    InfoModel info(InfoModel::Type::Vector, stream.TellI(), wxString::Format("Vector %d", counter++).c_str());

    x = stream.ReadFloat();

    y = -stream.ReadFloat();

    z = stream.ReadFloat();

    info.AppendBytes(x);
    info.AppendBytes(y);
    info.AppendBytes(z);

    desc.AppendInfo(info);
}
