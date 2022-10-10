// 2022/10/10 19:24:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/ICollection.h"
#include "Utils/FileInputStream.h"
#include "Models/Model.h"


uint ICollection::ReadUINT(pchar name)
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::UINT, stream.TellI(), name);

    uint result = stream.ReadUINT();

    DescriptionModel::Get()->AppendInfo(info.AppendBytes(result));

    return result;
}
