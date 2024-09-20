// 2022/09/23 20:23:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Archive.h"
#include "Resources/ResourceFactory.h"
#include "Utils/FileInputStream.h"
#include "Utils/Zlib.h"
#include "Resources/TranslatableResource.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


Archive::Archive(const wxString &path) : Archive(wxFileName(path))
{
}


Archive::Archive(const wxFileName &_file_name) : file_name(_file_name)
{
    if (file_name.GetExt() != "wd")
    {
        return;
    }

    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        return;
    }

    uint dirLn = file.ReadUINT(file.GetSize() - 4);

    wxMemoryBuffer dirData = file.ReadBytes(file.GetSize() - dirLn, dirLn);

    wxMemoryInputStream dirDataStream(dirData.GetData(), dirData.GetBufSize());     // Ётот данные дл€ декомпрессии

    wxZlibInputStream zstream(dirDataStream);                                       // Ёто поток декомпрессии

    wxMemoryOutputStream dir_stream;                                                // «десь декодированные данные

    zstream.Read(dir_stream);

    size_t size_dir_stream = dir_stream.GetSize();                                  // –азмер декодированных данных

    wxMemoryBuffer dir(size_dir_stream);

    dir_stream.CopyTo(dir.GetData(), size_dir_stream);                              // «десь декодированные данные

    wxMemoryInputStream stream((uint8 *)dir.GetData() + 10, dir.GetBufSize() - 10); // ѕоток декодированных данных, которые мы будем разбирать на ресурсы

    int counter = 0;

    while (!stream.Eof())
    {
        counter++;

        Resource resource = ResourceFactory::Create(stream);

        resources.emplace_back(resource);
    }

    counter = counter;
}


bool Archive::IsCorrectFile(const wxString &path)
{
    return wxFileName(path).GetExt() == "wd";
}


bool Archive::ReadContent()
{
    FileInputStream file(file_name.GetFullPath());

    if (!IsValidWDFile(file))
    {
        return false;
    }

    int counter_all = 0;
    int counter_decompressed = 0;

    for (Resource &resource : resources)
    {
        counter_all++;
        resource.data = file.ReadBytes(resource.info.offset, resource.info.length);

        if (resource.info.decompressedLength != resource.info.length)
        {
            resource.data = Zlib::Decompress(resource.data);
            counter_decompressed++;
        }
    }

    return true;
}


void Archive::Unpack(const wxString &path)
{
    wxFileName file(path);

    for (auto desc : resources)
    {
        if (desc.file_name.empty())
        {
            continue;
        }

        wxMemoryBuffer &data = desc.data;

        if (data.GetBufSize())
        {
            wxFileName path_resource(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name);

            if (!path_resource.GetExt().empty())
            {
                wxString dir = path_resource.GetPath();

                if (!wxDir::Exists(dir))
                {
                    wxDir::Make(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
                }

                wxFile file_resource;

                file_resource.Create(path_resource.GetFullPath(), true);

                file_resource.Write(data.GetData(), data.GetBufSize());

                if (desc.unknown_data.size())   
                {
                    wxFile file_unknown;

                    file_unknown.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".unknownData", true);

                    for (uint i = 0; i < desc.unknown_data.size(); i++)
                    {
                        file_unknown.Write(&desc.unknown_data[i], 1);
                    }
                }

                if (typeid(desc) == typeid(TranslatableResource))
                {
                    wxFile file_trans;

                    file_trans.Create(file.GetPath() + wxFileName::GetPathSeparator() + desc.file_name + ".translationId", true);

                    file_trans.Write(((TranslatableResource *)&desc)->translationID); //-V717
                }
            }
        }
    }
}


bool Archive::IsValidWDFile(wxFileInputStream &stream)
{
    wxZlibInputStream zstream(stream);

    uint8 buffer[8];
    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };

    zstream.Read(buffer, 8);

    return std::memcmp(buffer, template_buffer, 8) == 0;
}


void Archive::GetDescription(DescriptionArchive *description)
{
    description->file_name = wxString::Format("File : %s", file_name.GetFullPath().c_str());

    if (file_name.GetExt() == "wd")
    {
        int counter = 1;

        description->count_resources = resources.size();

        for (const Resource &resource : resources)
        {
            if (resource.file_name.empty())
            {
                description->AppendInfo({ counter++, std::string("Empty name resource") });

                continue;
            }

            if (resource.info.length)
            {
                description->AppendInfo({ counter++, resource.file_name.c_str(), resource.info.length, resource.info.decompressedLength });
            }
            else
            {
                description->AppendInfo({ counter++, "Empty resource" });
            }
        }
    }
}


bool DescriptionArchive::IsValid() const
{
    return false;
}


void DescriptionArchive::AppendInfo(const InfoArchive &info)
{
    push_back(info);
}


int DescriptionArchive::Size() const
{
    return (int)size();
}


void DescriptionArchive::DrawLine(const PageInfo *page, int y, int num_line) const
{
    const InfoArchive &desc = (*this)[num_line];

    int width = page->GetClientSize().GetWidth();

    page->DrawLine(0, y, 0, y + PageInfo::PIXELS_IN_LINE);
    page->DrawLine(0, y + PageInfo::PIXELS_IN_LINE, width, y + PageInfo::PIXELS_IN_LINE);
    page->DrawLine(width - 1, y, width - 1, y + PageInfo::PIXELS_IN_LINE);

    int x = DrawCell(page, 0, y, 35, wxString::Format("%d", desc.num_line));

    x = DrawCell(page, x, y, 390, desc.name);

    x = DrawCell(page, x, y, 50, wxString::Format("%d", desc.size));

    wxString text_size = (desc.size != desc.decompressed_size) ? wxString::Format("%d", desc.decompressed_size) : wxString("*");

    x = DrawCell(page, x, y, 55, text_size);

    DrawCell(page, x, y, 30, wxString::Format("%5.1f", (float)desc.decompressed_size / (float)desc.size));
}
