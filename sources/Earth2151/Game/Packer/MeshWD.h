// 2022/09/23 21:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/ResourceWD.h"


class MeshWD : public ResourceWD
{
public:

    MeshWD(const String<> &filename, ResourceInfoWD info, std::vector<uint8> *data) : ResourceWD(filename, info, data)
    {
    }

private:
};
