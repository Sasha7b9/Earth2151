// 2022/09/24 09:03:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"


namespace Packer
{
    class Group : public Resource
    {
    public:
        Group(std::string &filename, ResourceInfo, std::vector<uint8> *data);
    };
}

