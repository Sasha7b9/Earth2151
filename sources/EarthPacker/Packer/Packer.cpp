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

        if (directory.Make(fileName))
        {
            directory.Unpack(fileName.GetPath() + wxFileName::GetPathSeparator());
        }
    }
}


void Packer::CreateModel(const wxFileName &file_name)
{
    new Models::Model(file_name);
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

