// 2022/09/27 16:59:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Packer.h"
#include "Models/Model.h"
#include "Resources/Archive.h"
#include "Utils/Zlib.h"
#include "Resources/TranslatableResource.h"


void Packer::ProcessFile(const wxString &path)
{
    wxFileName file_name(path);

    if (file_name.GetExt() == "msh")
    {
        new Model(file_name);
    }
}
