// 2022/09/27 16:59:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Packer.h"
#include "Models/Model.h"
#include "Packer/Resources/ResourceDirectory.h"
#include "Utils/Zlib.h"
#include "Packer/Resources/TranslatableResource.h"


namespace Packer
{
    static void CreateModel(const wxFileName &);

    static bool GetDescriptionFileWD(const wxFileName &, DescriptionFile &);
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
        ResourceDirectory directory;

        MakeResourceDirectory(path, directory);
    }
}


void Packer::CreateModel(const wxFileName &file_name)
{
    new Models::Model(file_name);
}


bool Packer::MakeResourceDirectory(const wxString &path, ResourceDirectory &directory)
{
    wxFileName file_name(path);

    if (!directory.Make(file_name))
    {
        return false;
    }

    FileInputStream file(file_name.GetFullPath());

    for each (auto desc in directory.resources)
    {
        if (desc.file_name.empty())
        {
            continue;
        }

        wxMemoryBuffer &data = desc.data;

        if (data.GetBufSize())
        {
            wxFileName path_resource(file_name.GetPath() + wxFileName::GetPathSeparator() + desc.file_name);

            wxString dir = path_resource.GetPath();

            if (!wxDir::Exists(dir))
            {
                wxDir::Make(dir);
            }

            wxFile file_resource;

            file_resource.Create(path_resource.GetFullPath(), true);

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

                file_trans.Write(((Packer::TranslatableResource *)&desc)->translationID); //-V717
            }
        }
    }

    return true;
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


bool Packer::GetDescriptionFileWD(const wxFileName &file_name, DescriptionFile &description)
{
    ResourceDirectory dirdesc;

    if (!dirdesc.Make(file_name))
    {
        return false;
    }

    FileInputStream file(file_name.GetFullPath());

    int counter = 1;

    uint num_resources = dirdesc.resources.size();

    description.AppendLine(wxString::Format("%d resources", num_resources));

    for each (const Resource &resource in dirdesc.resources)
    {
        if (resource.file_name.empty())
        {
            description.AppendLine(wxString::Format("%d : Empty name resource", counter++));

            continue;
        }

        if (resource.info.length)
        {
            wxMemoryBuffer data = file.ReadBytes(resource.info.offset, resource.info.length);

            description.AppendLine(wxString::Format("%d : %s %d %d", counter++, resource.file_name.c_str(), resource.info.length, resource.info.decompressedLength));
        }
        else
        {
            description.AppendLine(wxString::Format("%d : Empty resource",  counter++));
        }
    }

    return true;
}


void Packer::GetDescriptionFileMSH(const wxFileName &, DescriptionFile &)
{

}

