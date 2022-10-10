// 2022/09/24 20:55:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Face.h"
#include "Models/Model.h"


Face::Face(int num)
{
    InfoModel info(InfoModel::Type::Face, stream->TellI(), wxString::Format("face %d", num).c_str());

    v1 = stream->Read2Bytes();
    v2 = stream->Read2Bytes();
    v3 = stream->Read2Bytes();
    unknown = stream->Read2Bytes();

    info.AppendBytes(&v1, 2);
    info.AppendBytes(&v2, 2);
    info.AppendBytes(&v3, 2);
    info.AppendBytes(&unknown, 2);

    desc->AppendInfo(info, stream);
}
