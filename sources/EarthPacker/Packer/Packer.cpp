// 2022/09/27 16:59:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Packer.h"
#include "Models/Model.h"
#include "Packer/Resources/Directory.h"
#include "Utils/Zlib.h"
#include "Packer/Resources/TranslatableResource.h"


namespace Packer
{
    static bool IsValidWDFile(wxFileInputStream &);

    static void CreateModel(const wxFileName &);

    static void UnpackFile(const wxFileName &);
}


void Packer::ProcessFile(const wxString &path)
{
    wxFileName fileName(path);

    if (fileName.GetExt() == "msh")
    {
        CreateModel(fileName);
    }
    else if (fileName.GetExt() == "wd")
    {
        UnpackFile(fileName);
    }
}


void Packer::CreateModel(const wxFileName &file_name)
{
    new Models::Model(file_name);
}


void Packer::UnpackFile(const wxFileName &file_name)
{
    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        return;
    }

    uint dirLn = file.ReadUINT(file.GetSize() - 4);

    wxMemoryBuffer dirData = file.ReadBytes(file.GetSize() - dirLn, dirLn);

    wxMemoryInputStream dirDataStream(dirData.GetData(), dirData.GetBufSize());

    wxZlibInputStream zstream(dirDataStream);

    wxMemoryOutputStream dir_stream;

    zstream.Read(dir_stream);

    size_t size_dir_stream = dir_stream.GetSize();

    wxMemoryBuffer _dir(size_dir_stream);

    dir_stream.CopyTo(_dir.GetData(), size_dir_stream);

    auto dirdesc = new Packer::Directory(_dir);

    for each (auto desc in dirdesc->resources)
    {
        if (desc.file_name.empty())
        {
            continue;
        }

        wxMemoryBuffer data = file.ReadBytes(desc.info.offset, desc.info.length);

        if (data.GetBufSize())
        {
            wxFileName path_resource(file_name.GetPath() + wxFileName::GetPathSeparator() + desc.file_name);

            wxString directory = path_resource.GetPath();

            if (!wxDir::Exists(directory))
            {
                wxDir::Make(directory);
            }

            wxFile file_resource;

            file_resource.Create(path_resource.GetFullPath(), true);

            data = (desc.info.decompressedLength == desc.info.length) ? data : Zlib::Decompress(data);

            file_resource.Write(data.GetData(), data.GetBufSize());

            if (desc.unknown_data.size())
            {
                wxFile file_unknown;

                file_unknown.Create(file_name.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".unknownData", true);

                for (uint i = 0; i < desc.unknown_data.size(); i++)
                {
                    file_unknown.Write(&desc.unknown_data[i], 1);
                }
            }

            if (typeid(desc) == typeid(Packer::TranslatableResource))
            {
                wxFile file_trans;

                file_trans.Create(file_name.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".translationId", true);

                file_trans.Write(((Packer::TranslatableResource *)&desc)->translationID);
            }
        }
    }

    delete dirdesc;
}


bool Packer::IsValidWDFile(wxFileInputStream &stream)
{
    wxZlibInputStream zstream(stream);

    uint8 buffer[8];
    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };

    zstream.Read(buffer, 8);

    return std::memcmp(buffer, template_buffer, 8) == 0;
}


void Packer::GetDescriptionFile(const wxString &path, DescriptionFile &description)
{
    wxFileName file_name(path);

    if (file_name.GetExt().empty())
    {
        return;
    }

    description.AppendLine(wxString::Format("File : %s", path.c_str()));
}
