// 2022/09/24 19:43:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/ModelPart.h"


Models::ModelPart::ModelPart(FileInputStream &stream)
{
    vertices.Create(stream);
    skipParent = stream.ReadByte();
    unknownFlag = stream.ReadByte();
    stream.Read2Bytes();

    auto pos = stream.TellI();

    texture.Create(stream);
    faces.Create(stream);
    animations.Create(stream);
    unknownValue = stream.ReadINT();
    offset.Create(stream);
    stream.Read(unknown_bytes, 5);
}
