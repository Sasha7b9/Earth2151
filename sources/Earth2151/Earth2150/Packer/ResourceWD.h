// 2022/09/23 20:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/MemoryBufferWD.h"
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
    ResourceWD(const String<> &filename, ResourceInfoWD, std::vector<uint8> * = nullptr);

    String<>           file_name;
    ResourceInfoWD     info;
    MemoryBufferWD     data;
    std::vector<uint8> unknown_data;
};
