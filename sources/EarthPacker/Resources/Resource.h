// 2022/09/23 20:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Resources
{
    struct ResourceInfo
    {
        uint i1;
        uint i2;
        uint i3;
    };

    class Resource
    {
    public:
        Resource(std::string &filename, ResourceInfo, wxMemoryBuffer * = nullptr);
    };
}
