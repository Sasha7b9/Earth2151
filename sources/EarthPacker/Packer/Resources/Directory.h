// 2022/09/23 20:23:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"


namespace Packer
{
    class Directory
    {
    public:
        Directory(wxMemoryBuffer &);

        std::vector<Resource> resources;

    private:
        void ReadFileDescriptors(wxMemoryInputStream &);
    };
}
