// 2022/09/24 19:43:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/ModelPart.h"
#include "Models/Model.h"


ModelPart::ModelPart(DescriptionModel &desc, int num_model)
{
    InfoModel info(InfoModel::Type::ModelPart, stream->TellI(), wxString::Format(" Model part %d", num_model).c_str());

    vertices.Create(desc);
    skipParent = stream->ReadByte();
    unknownFlag = stream->ReadByte();
    stream->Read2Bytes();
    texture.Create();
    faces.Create(desc);
    animations.Create(desc);
    unknownValue = stream->ReadINT();
    offset.Create(desc, "offset");
    stream->Read(unknown_bytes, 5);

    info.size = (int)stream->TellI() - info.header.offset;

    desc.AppendInfo(info);
}
