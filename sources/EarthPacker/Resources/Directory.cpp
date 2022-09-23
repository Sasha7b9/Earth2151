// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Directory.h"
#include "Resources/ResourceFactory.h"


using namespace Resources;


Directory::Directory(wxMemoryBuffer &directoryData)
{
    wxMemoryInputStream stream((uint8 *)directoryData.GetData() + 10, directoryData.GetBufSize() - 10);
}


void Directory::ReadFileDescriptors(wxMemoryInputStream &stream)
{
    while (!stream.Eof())
    {
        resources.push_back(ResourceFactory::Create(stream));
    }
}
