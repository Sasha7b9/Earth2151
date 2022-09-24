// 2022/09/24 15:29:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Models/Collections/Vertices.h"
#include "Models/Elements/TextureInfo.h"


namespace Models
{
    class ModelPart
    {
    public:
        class ModelPart(FileInputStream &);
    private:
        Vertices vertices;
        int skipParent;
        int unknownFlag;
        TextureInfo texture;
    };
}
