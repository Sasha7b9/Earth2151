// 2022/10/02 20:38:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Packer
{
    class ArchiveWD;
}


/*
*  Текстовое описание разбираемого файла
*/

class DescriptionFile : public std::vector<std::string>
{
    friend class Packer::ArchiveWD;

public:
    bool IsValid() const;
    void AppendLine(pchar);
    int Size();
private:
    std::string file_name;
    int count_resources;        // Количество файлов в архиве
};
