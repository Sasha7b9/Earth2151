// 2022/09/24 21:03:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Animations.h"
#include "Models/Model.h"


void PositionOffsetFrames::Create(FileInputStream &stream, DescriptionModel &desc, bool unknown)
{
    int length = 0;
    if (unknown)
    {
        length = ReadUINT("unkn frames");
    }
    else
    {
        length = ReadUINT("trans frames");
    }

    for (int i = 0; i < length; i++)
    {
        if (unknown)
        {
            push_back(Vector(stream, desc, wxString::Format("unkn a %d", i).c_str()));
        }
        else
        {
            push_back(Vector(stream, desc, wxString::Format("pos a %d", i).c_str()));
        }
    }
}


uint PositionOffsetFrames::ReadUINT(pchar name)
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::UINT, stream.TellI(), name);

    uint result = stream.ReadUINT();

    DescriptionModel::Get()->AppendInfo(info.AppendBytes(result));

    return result;
}


void RotationFrames::Create()
{
    int length = ReadUINT("rot frames");

    for (int i = 0; i < length; i++)
    {
        push_back(RotationFrame(i));
    }
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
    unknownAnimationData.Create(stream, desc, true);
    movementFrames.Create(stream, desc, false);
    rotationFrames.Create();
}
