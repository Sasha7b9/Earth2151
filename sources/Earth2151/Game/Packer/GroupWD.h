// 2022/09/24 09:03:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/ResourceWD.h"


class GroupWD : public ResourceWD
{
public:
    GroupWD(const String<> &filename, ResourceInfoWD info, std::vector<uint8> *data) :
        ResourceWD(filename, info, data)
    {
    }
};
