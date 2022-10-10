// 2022/09/24 21:09:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/RotationFrame.h"
#include "Models/Model.h"


RotationFrame::RotationFrame(int num_frame)
{
    DescriptionModel &desc = *DescriptionModel::Get();

    InfoModel info(InfoModel::Type::Mat44, stream->TellI(), wxString::Format("rot a %d", num_frame));

    (*this)[0][0] = stream->ReadFloat();
    (*this)[0][1] = stream->ReadFloat();
    (*this)[0][2] = stream->ReadFloat();
    (*this)[0][3] = stream->ReadFloat();
    (*this)[1][0] = stream->ReadFloat();
    (*this)[1][1] = stream->ReadFloat();
    (*this)[1][2] = stream->ReadFloat();
    (*this)[1][3] = stream->ReadFloat();
    (*this)[2][0] = stream->ReadFloat();
    (*this)[2][1] = stream->ReadFloat();
    (*this)[2][2] = stream->ReadFloat();
    (*this)[2][3] = stream->ReadFloat();
    (*this)[3][0] = stream->ReadFloat();
    (*this)[3][1] = stream->ReadFloat();
    (*this)[3][2] = stream->ReadFloat();
    (*this)[3][3] = stream->ReadFloat();

    info.AppendBytes((*this)[0][0]);
    info.AppendBytes((*this)[0][1]);
    info.AppendBytes((*this)[0][2]);
    info.AppendBytes((*this)[0][3]);
    info.AppendBytes((*this)[1][0]);
    info.AppendBytes((*this)[1][1]);
    info.AppendBytes((*this)[1][2]);
    info.AppendBytes((*this)[1][3]);
    info.AppendBytes((*this)[2][0]);
    info.AppendBytes((*this)[2][1]);
    info.AppendBytes((*this)[2][2]);
    info.AppendBytes((*this)[2][3]);
    info.AppendBytes((*this)[3][0]);
    info.AppendBytes((*this)[3][1]);
    info.AppendBytes((*this)[3][2]);
    info.AppendBytes((*this)[3][3]);

    desc.AppendInfo(info);
}
