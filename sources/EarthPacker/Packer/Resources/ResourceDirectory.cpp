// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/ResourceDirectory.h"
#include "Packer/Resources/ResourceFactory.h"
#include "Utils/FileInputStream.h"


bool Packer::ResourceDirectory::Make(const wxFileName &file_name)
{
    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        return false;
    }

    uint dirLn = file.ReadUINT(file.GetSize() - 4);

    wxMemoryBuffer dirData = file.ReadBytes(file.GetSize() - dirLn, dirLn);

    wxMemoryInputStream dirDataStream(dirData.GetData(), dirData.GetBufSize());

    wxZlibInputStream zstream(dirDataStream);

    wxMemoryOutputStream dir_stream;

    zstream.Read(dir_stream);

    size_t size_dir_stream = dir_stream.GetSize();

    wxMemoryBuffer dir(size_dir_stream);

    dir_stream.CopyTo(dir.GetData(), size_dir_stream);

    wxMemoryInputStream stream((uint8 *)dir.GetData() + 10, dir.GetBufSize() - 10);

    ReadResourcesDescriptors(stream);

    return true;
}


bool Packer::ResourceDirectory::IsValidWDFile(wxFileInputStream &stream)
{
    wxZlibInputStream zstream(stream);

    uint8 buffer[8];
    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };

    zstream.Read(buffer, 8);

    return std::memcmp(buffer, template_buffer, 8) == 0;
}


void Packer::ResourceDirectory::ReadResourcesDescriptors(wxMemoryInputStream &stream)
{
    while (!stream.Eof())
    {
        resources.emplace_back(ResourceFactory::Create(stream));
    }
}
