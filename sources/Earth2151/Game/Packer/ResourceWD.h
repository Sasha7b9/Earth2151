// 2022/09/23 20:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/MemoryBufferWD.h"
#include <vector>


struct ResourceInfoWD
{
    int offset = 0;
    int length = 0;
    int decompressedLength = 0;
};


class ResourceWD
{
public:

    ResourceWD(const String<> &filename, ResourceInfoWD _info, std::vector<uint8> *_unknown_data = nullptr) :
        file_name(filename), info(_info)
    {
        if (_unknown_data)
        {
            unknown_data = *_unknown_data;
        }
    }

    String<>           file_name;
    ResourceInfoWD     info;
    MemoryBufferWD     data;
    std::vector<uint8> unknown_data;
};
