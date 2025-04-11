// 2022/09/14 14:35:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>
#include <list>
#include <chrono>


struct FileInfo
{
    std::string name;
    size_t      size;
    uint        crc;
    bool        processed;  // Если false - файл не обработан

    FileInfo() : name(), size(0), crc(0), processed(false) {};
    FileInfo(pchar _name) : FileInfo() { name = _name; };
    FileInfo(const FileInfo &);
    bool operator==(const FileInfo &info);
    FileInfo &operator=(const FileInfo &);
    virtual ~FileInfo();
};


struct ListFiles
{
    ListFiles()
    {
        for (int i = 0; i < maxThreads; i++)
        {
            runThread[i] = false;
        }
    }

    ListFiles(pchar directory, std::vector<std::string> *ignoredFiles, std::vector<std::string> *ignoredExtensions, std::vector<std::string> *ignoredDirs)
    {
        for (int i = 0; i < maxThreads; i++)
        {
            runThread[i] = false;
        }

        Build(directory, ignoredFiles, ignoredExtensions, ignoredDirs);
    }

    std::list<FileInfo> files;

    // Заполняет вектор данными файлов из каталога directory
    void Build(pchar directory, std::vector<std::string> *ignoredFiles, std::vector<std::string> *ignoredExtensions, std::vector<std::string> *ignoredDirs);

    // Заполнить вектор данными из файла
    bool ExtractFromFile(pchar fileName);

    void ExtractFilesNames(std::vector<std::string> &);

    // Возвращает true, если в списке существует файл с именем file
    bool ExistFile(const std::string &file) const;

    // Возвращает true, если в списке существует файл со свойствами, соответствующими info
    bool ExistEqualFile(const FileInfo &info) const;

    // Запись списка файлов в файл
    void WriteToFile(pchar fileName);

private:

    void AppendSubDirectory(pchar directory, std::vector<std::string> *ignoredFiles = nullptr, std::vector<std::string> *ignoredExtensions = nullptr, std::vector<std::string> *ignoredDirs = nullptr);

    void AppendFile(std::string &fullName, const std::vector<std::string> *ignoredFiles = nullptr, const std::vector<std::string> *ignoredExtensions = nullptr);

    // Возвращает true, если fullName заканчивается на одно из расширений из списка ignoredExtensions
    bool ExtensionIs(const std::string &fullName, const std::vector<std::string> *ignoredExtensions);

    // Возвращает true, если fullName есть одно из значений, содержащихсся в ignoredFiles
    bool FileIs(const std::string &fullName, const std::vector<std::string> *ignoredFiles);

    // Возвращает количество действующих в данный момент потоков
    int NumThreads();

    static const int maxThreads = 8;        // Максимальное число потоков

    bool runThread[maxThreads];             // Установленное в true значение означает, что данный поток находится в работе

    std::chrono::steady_clock::time_point startBuild;                 // Время начала построения списка файлов

    std::chrono::seconds prevSeconds = std::chrono::seconds::zero();
};
