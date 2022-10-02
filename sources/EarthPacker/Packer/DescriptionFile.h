// 2022/10/02 20:38:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  Текстовое описание разбираемого файла
*/

class DescriptionFile : public std::vector<std::string>
{
public:
    bool IsValid() const;
    void AppendLine(pchar);
private:
};
