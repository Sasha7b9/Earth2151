// 2022/09/23 20:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Packer
{
    struct ResourceInfo
    {
        int offset = 0;
        int length = 0;
        int decompressedLength = 0;
    };

    class Resource
    {
    public:
        Resource(std::string &filename, ResourceInfo, std::vector<uint8> * = nullptr);

        std::string file_name;
        ResourceInfo info;
        wxMemoryBuffer data;
        std::vector<uint8> unknown_data;
    };
}
