// 2022/09/27 16:59:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/DescriptionFile.h"


namespace Packer
{
    void ProcessFile(const wxString &);

    void GetDescriptionFile(const wxString &path, DescriptionFile &out);
}
