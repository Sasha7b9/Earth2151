// 2022/09/24 09:06:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"


namespace Packer
{
    class TranslatableResource : public Resource
    {
    public:
        TranslatableResource(std::string &filename, ResourceInfo, std::string translationID, std::vector<uint8> * = nullptr);
        std::string translationID;
    };
}