// 2022/09/23 20:23:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"
#include "Packer/DescriptionFile.h"


namespace Packer
{
    class ArchiveWD
    {
    public:

        ArchiveWD(const wxFileName &);
        ArchiveWD(const wxString &path);

        // Читать содержимое
        bool ReadContent();

        // Распаковать содержимое
        void Unpack(const wxString &path);

        void GetDescription(DescriptionFileWD &out);

        std::vector<Resource> resources;

        static bool IsCorrectFile(const wxString &path);

    private:

        wxFileName file_name;

        bool IsValidWDFile(wxFileInputStream &);
    };
}
