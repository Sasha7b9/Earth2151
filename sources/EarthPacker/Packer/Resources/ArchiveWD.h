// 2022/09/23 20:23:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"


namespace Packer
{
    class ArchiveWD
    {
    public:

        // Читать оглавление
        bool ReadDescriptor(const wxFileName &);

        // Читать содержимое
        bool ReadContent();

        // Распаковать содержимое
        void Unpack(const wxString &path);

        std::vector<Resource> resources;

        bool IsValid() const { return resources.size() != 0; }

    private:

        wxFileName file_name;

        bool IsValidWDFile(wxFileInputStream &);
    };
}
