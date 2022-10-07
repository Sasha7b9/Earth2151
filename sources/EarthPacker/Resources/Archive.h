// 2022/09/23 20:23:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Resources/Resource.h"
#include "Resources/Description.h"


struct InfoArchive
{
    InfoArchive(int _num_line, const std::string &_name, int _size = 0, int _decompressed_size = 0) :
        num_line(_num_line), name(_name), size(_size), decompressed_size(_decompressed_size) {}
    int num_line;
    std::string name;
    int size;
    int decompressed_size;
};


/*
*  Текстовое описание разбираемого файла
*/

class DescriptionArchive : public std::vector<InfoArchive>, public Description
{
    friend class Archive;

public:
    bool IsValid() const;
    void AppendInfo(const InfoArchive &);
    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_line) const override;
private:
    std::string file_name;
    int count_resources;        // Количество файлов в архиве
};


class Archive
{
public:

    Archive(const wxFileName &);
    Archive(const wxString &path);

    // Читать содержимое
    bool ReadContent();

    // Распаковать содержимое
    void Unpack(const wxString &path);

    void GetDescription(DescriptionArchive *out);

    std::vector<Resource> resources;

    static bool IsCorrectFile(const wxString &path);

private:

    wxFileName file_name;

    bool IsValidWDFile(wxFileInputStream &);
};
