// 2022/09/23 20:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Resources
{
    struct ResourceInfo
    {
        uint offset;
        uint length;
        uint decompressedLength;
    };

    class Resource
    {
    public:
        Resource(std::string &filename, ResourceInfo, std::vector<uint8> * = nullptr);
    };
}
