// 2022/09/24 21:03:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Animations.h"
#include "Models/Model.h"


void PositionOffsetFrames::Create(FileInputStream &stream, DescriptionModel &desc)
{
    int length = stream.ReadINT();

    for (int i = 0; i < length; i++)
    {
        push_back(Vector(stream, desc, wxString::Format("anim p %d", i).c_str()));
    }
}


void RotationFrames::Create()
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::RotationFrames, stream.TellI(), " Rot frames");

    int length = ReadUINT("num frames");

    for (int i = 0; i < length; i++)
    {
        push_back(RotationFrame(i));
    }

    info.size = (int)stream.TellI() - info.header.offset;

    DescriptionModel::Get()->AppendInfo(info);
}


uint RotationFrames::ReadUINT(pchar name)
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::UINT, stream.TellI(), name);

    uint result = stream.ReadUINT();

    DescriptionModel::Get()->AppendInfo(info.AppendBytes(result));

    return result;
}


void Animations::Create(FileInputStream &stream, DescriptionModel &desc)
{
    unknownAnimationData.Create(stream, desc);
    movementFrames.Create(stream, desc);
    rotationFrames.Create();
}
