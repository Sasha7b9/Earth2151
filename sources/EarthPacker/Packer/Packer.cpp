// 2022/09/27 16:59:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Packer.h"
#include "Models/Model.h"
#include "Packer/Resources/ArchiveWD.h"
#include "Utils/Zlib.h"
#include "Packer/Resources/TranslatableResource.h"


namespace Packer
{
    static void CreateModel(const wxFileName &);

    static bool GetDescriptionFileWD(const ArchiveWD &, DescriptionFile &);
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
        ArchiveWD arch(fileName);

        arch.ReadContent();

        arch.Unpack(fileName.GetPath() + wxFileName::GetPathSeparator());
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
        ArchiveWD arch(file_name);

        GetDescriptionFileWD(arch, description);
    }
}


bool Packer::GetDescriptionFileWD(const ArchiveWD &arch, DescriptionFile &description)
{
    int counter = 1;

    description.AppendLine(wxString::Format("%d resources", arch.resources.size()));

    for each (const Resource &resource in arch.resources)
    {
        if (resource.file_name.empty())
        {
            description.AppendLine(wxString::Format("%d : Empty name resource", counter++));

            continue;
        }

        if (resource.info.length)
        {
            description.AppendLine(wxString::Format("%d : %s %d %d", counter++, resource.file_name.c_str(), resource.info.length, resource.info.decompressedLength));
        }
        else
        {
            description.AppendLine(wxString::Format("%d : Empty resource",  counter++));
        }
    }

    return true;
}
