// 2022/10/02 20:38:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DescInfoWD
{
    DescInfoWD(int _num_line, const std::string &_name, int _size = 0, int _decompressed_size = 0) :
        num_line(_num_line), name(_name), size(_size), decompressed_size(_decompressed_size) {}
    int num_line;
    std::string name;
    int size;
    int decompressed_size;
};


/*
*  Текстовое описание разбираемого файла
*/

class DescriptionFileWD : public std::vector<DescInfoWD>
{
    friend class ArchiveWD;

public:
    bool IsValid() const;
    void AppendLine(const DescInfoWD &);
    int Size();
private:
    std::string file_name;
    int count_resources;        // Количество файлов в архиве
};
