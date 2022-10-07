// 2022/09/24 15:29:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Models/Collections/Vertices.h"
#include "Models/Elements/TextureInfo.h"
#include "Models/Collections/Faces.h"
#include "Models/Collections/Animations.h"


struct DescriptionModel;


class ModelPart
{
public:
    class ModelPart(FileInputStream &, DescriptionModel &, int num_model);

    Vertices vertices;
    int skipParent;
    int unknownFlag;
    TextureInfo texture;
    Faces faces;
    Animations animations;
    int unknownValue;
    Vector offset;
    uint8 unknown_bytes[5];
};
