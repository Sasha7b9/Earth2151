// 2022/09/24 09:06:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Resources/TranslatableResource.h"


namespace Resources
{
    class Terrain : public TranslatableResource
    {
    public:
        Terrain(std::string &filename, ResourceInfo, std::string &translationID, std::vector<uint8> *data);
    };
}
