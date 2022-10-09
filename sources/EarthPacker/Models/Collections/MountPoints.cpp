// 2022/09/24 16:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/MountPoints.h"
#include "Models/Model.h"


MountPoints::MountPoints(FileInputStream &stream, DescriptionModel &desc)
{
    InfoModel info(InfoModel::Type::MountPoints, stream.TellI(), "Mount points");

    for (int i = 0; i < NUMBER_OF_MOUNTPOINTS; i++)
    {
        Vector vector(stream, desc);

        push_back(vector);
    }

    info.size = (int)stream.TellI() - info.header.offset;

    desc.AppendInfo(info);
}
