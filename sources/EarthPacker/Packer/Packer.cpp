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

    static bool UnpackFile(const wxFileName &);

    static void GetDescriptionFileWD(const wxFileName &, DescriptionFile &);
    static void GetDescriptionFileMSH(const wxFileName &, DescriptionFile &);
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


bool Packer::UnpackFile(const wxFileName &file_name)
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

    wxMemoryBuffer _dir(size_dir_stream);

    dir_stream.CopyTo(_dir.GetData(), size_dir_stream);

    Directory *dirdesc = new Packer::Directory(_dir);

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

    return true;
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

    if (file_name.GetExt() == "wd")
    {
        GetDescriptionFileWD(file_name, description);
    }
    else if (file_name.GetExt() == "msh")
    {
        GetDescriptionFileMSH(file_name, description);
    }
}


void Packer::GetDescriptionFileWD(const wxFileName &file_name, DescriptionFile &description)
{
    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        description.AppendLine("Is not valid WD file");

        return;
    }

    uint dir_len = file.ReadUINT(file.GetSize() - 4);

    description.AppendLine(wxString::Format("dir_len = %d", dir_len));

    wxMemoryBuffer dir_data = file.ReadBytes(file.GetSize() - dir_len, dir_len);

    wxMemoryInputStream dirDataStream(dir_data.GetData(), dir_data.GetBufSize());

    wxZlibInputStream zstream(dirDataStream);

    wxMemoryOutputStream dir_stream;

    zstream.Read(dir_stream);

    size_t size_dir_stream = dir_stream.GetSize();

    wxMemoryBuffer _dir(size_dir_stream);

    dir_stream.CopyTo(_dir.GetData(), size_dir_stream);

    Directory *dirdesc = new Packer::Directory(_dir);

    int counter = 1;

    description.AppendLine(wxString::Format("%d resources", dirdesc->resources.size()));

    for each (const Resource &resource in dirdesc->resources)
    {
        if (resource.file_name.empty())
        {
            description.AppendLine(wxString::Format("%d : Empty name resource", counter++));

            continue;
        }

        wxMemoryBuffer data = file.ReadBytes(resource.info.offset, resource.info.length);

        if (data.GetBufSize())
        {
            description.AppendLine(wxString::Format("%d : %s %d %d", counter++, resource.file_name.c_str(), resource.info.length, resource.info.decompressedLength));
        }
        else
        {
            description.AppendLine(wxString::Format("%d : Empty buffer", counter++));
        }
    }

    delete dirdesc;
}


void Packer::GetDescriptionFileMSH(const wxFileName &file_name, DescriptionFile &description)
{

}

