// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/ArchiveWD.h"
#include "Packer/Resources/ResourceFactory.h"
#include "Utils/FileInputStream.h"
#include "Utils/Zlib.h"
#include "Packer/Resources/TranslatableResource.h"


bool Packer::ArchiveWD::ReadDescriptor(const wxFileName &_file_name)
{
    file_name = _file_name;

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

    while (!stream.Eof())
    {
        Resource resource = ResourceFactory::Create(stream);

        resources.emplace_back(resource);
    }

    return true;
}


bool Packer::ArchiveWD::ReadContent()
{
    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        return false;
    }

    for (Resource &resource : resources)
    {
        resource.data = file.ReadBytes(resource.info.offset, resource.info.length);

        if (resource.info.decompressedLength != resource.info.length)
        {
            resource.data = Zlib::Decompress(resource.data);
        }
    }

    return true;
}


void Packer::ArchiveWD::Unpack(const wxString &path)
{
    wxFileName file(path);

    for each (auto desc in resources)
    {
        if (desc.file_name.empty())
        {
            continue;
        }

        wxMemoryBuffer &data = desc.data;

        if (data.GetBufSize())
        {
            wxFileName path_resource(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name);

            if (!path_resource.GetExt().empty())
            {
                wxString dir = path_resource.GetPath();

                if (!wxDir::Exists(dir))
                {
                    wxDir::Make(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
                }

                wxFile file_resource;

                wxString _path_resource = path_resource.GetFullPath();

                file_resource.Create(_path_resource, true);

                void *_data = data.GetData();
                size_t _size = data.GetBufSize();

                file_resource.Write(data.GetData(), data.GetBufSize());

                if (desc.unknown_data.size())   
                {
                    wxFile file_unknown;

                    file_unknown.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".unknownData", true);

                    for (uint i = 0; i < desc.unknown_data.size(); i++)
                    {
                        file_unknown.Write(&desc.unknown_data[i], 1);
                    }
                }

                if (typeid(desc) == typeid(Packer::TranslatableResource))
                {
                    wxFile file_trans;

                    file_trans.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".translationId", true);

                    file_trans.Write(((Packer::TranslatableResource *)&desc)->translationID); //-V717
                }
            }
        }
    }
}


bool Packer::ArchiveWD::IsValidWDFile(wxFileInputStream &stream)
{
    wxZlibInputStream zstream(stream);

    uint8 buffer[8];
    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };

    zstream.Read(buffer, 8);

    return std::memcmp(buffer, template_buffer, 8) == 0;
}