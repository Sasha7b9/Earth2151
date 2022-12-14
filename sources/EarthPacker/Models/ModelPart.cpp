// 2022/09/24 19:43:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/ModelPart.h"
#include "Models/Model.h"


ModelPart::ModelPart(int num_model)
{
    InfoModel info(InfoModel::Type::ModelPart, stream->TellI(), wxString::Format(" Model part %d", num_model).c_str());

    vertices.Create();
    skipParent = ReadByte("Skip parent");
    unknownFlag = ReadByte("Unknown flag");
    ReadUINT16("Unknown uint16");
    texture.Create();
    faces.Create();
    animations.Create();
    unknownValue = (int)ReadUINT("Unknown value");
    offset.Create("offset");
    stream->Read(unknown_bytes, 5);

    info.size = (int)stream->TellI() - info.header.offset;

    desc->AppendInfo(info);
}
