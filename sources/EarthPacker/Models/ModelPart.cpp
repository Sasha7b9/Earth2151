// 2022/09/24 19:43:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/ModelPart.h"
#include "Models/Model.h"


ModelPart::ModelPart(FileInputStream &stream, DescriptionModel &desc, int num_model)
{
    InfoModel info(stream.TellI(), 0, wxString::Format("Model part %d", num_model).c_str());

    int position = desc.Size();

    vertices.Create(stream, desc);
    skipParent = stream.ReadByte();
    unknownFlag = stream.ReadByte();
    stream.Read2Bytes();
    texture.Create(stream);
    faces.Create(stream, desc);
    animations.Create(stream);
    unknownValue = stream.ReadINT();
    offset.Create(stream);
    stream.Read(unknown_bytes, 5);

    info.size = (int)stream.TellI() - info.address;

    desc.InsertInfo(position, info);
}
