// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/Directory.h"
#include "Packer/Resources/ResourceFactory.h"


Packer::Directory::Directory(wxMemoryBuffer &directoryData)
{
    wxMemoryInputStream stream((uint8 *)directoryData.GetData() + 10, directoryData.GetBufSize() - 10);

    ReadFileDescriptors(stream);
}


void Packer::Directory::ReadFileDescriptors(wxMemoryInputStream &stream)
{
    while (!stream.Eof())
    {
        resources.emplace_back(ResourceFactory::Create(stream));
    }
}
