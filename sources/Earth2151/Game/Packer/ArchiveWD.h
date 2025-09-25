// 2022/09/23 20:23:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/ResourceWD.h"


struct InfoArchive
{
    InfoArchive(int _num_line, const std::string &_name, int _size = 0, int _decompressed_size = 0) :
        num_line(_num_line), name(_name), size(_size), decompressed_size(_decompressed_size)
    {
    }
    int num_line;
    std::string name;
    int size;
    int decompressed_size;
};


/*
*  Текстовое описание разбираемого файла
*/

class DescriptionArchive : public std::vector<InfoArchive>
{
    friend class ArchiveWD;

public:
    bool IsValid() const;
    void AppendInfo(const InfoArchive &);
    int Size() const;
private:
    std::string file_name;
    int count_resources;        // Количество файлов в архиве
};


class ArchiveWD
{
public:

    ArchiveWD(pchar full_path);

    // Читать содержимое
    bool ReadContent();

    // Распаковать содержимое
    void Unpack(const String<> &path);

private:

    void GetDescription(DescriptionArchive *out);

    std::vector<ResourceWD> resources;

    static bool IsCorrectFile(const String<> &path);

    String<> file_name;

    bool IsValidWDFile(File &);
};
