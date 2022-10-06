// 2022/09/27 16:59:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Packer.h"
#include "Models/Model.h"
#include "Resources/ArchiveWD.h"
#include "Utils/Zlib.h"
#include "Resources/TranslatableResource.h"


namespace Packer
{
    static void CreateModel(const wxFileName &);
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
        Archive arch(fileName);

        arch.ReadContent();

        arch.Unpack(fileName.GetPath() + wxFileName::GetPathSeparator());
    }
}


void Packer::CreateModel(const wxFileName &file_name)
{
    new Models::Model(file_name);
}
