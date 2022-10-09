// 2022/09/24 20:55:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Face.h"
#include "Models/Model.h"


Face::Face(FileInputStream &stream, DescriptionModel &desc)
{
    InfoModel info(InfoModel::Type::Face, stream.TellI(), "Face");

    v1 = stream.Read2Bytes();
    v2 = stream.Read2Bytes();
    v3 = stream.Read2Bytes();
    unknown = stream.Read2Bytes();

    info.AppendBytes(&v1, 2);
    info.AppendBytes(&v2, 2);
    info.AppendBytes(&v3, 2);
    info.AppendBytes(&unknown, 2);

    desc.AppendInfo(info, stream);
}
