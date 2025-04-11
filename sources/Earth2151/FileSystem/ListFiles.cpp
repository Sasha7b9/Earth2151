// 2022/09/14 14:35:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/ListFiles.h"
#include "Utils/StringUtils.h"
#include "Utils/SystemUtils.h"
#include <iostream>

#ifdef WIN32
#else
#include "Base/C4PrefixLinux.h"
#endif

#include <sstream>
#include <thread>
#include <chrono>


FileInfo::FileInfo(const FileInfo &rhs)
{
    name = rhs.name;
    size = rhs.size;
    crc = rhs.crc;
    processed = rhs.processed;
}


FileInfo &FileInfo::operator=(const FileInfo &rhs)
{
    name = rhs.name;
    size = rhs.size;
    crc = rhs.crc;
    processed = rhs.processed;
    return *this;
}

FileInfo::~FileInfo()
{
}


bool ListFiles::ExistFile(const std::string &file) const
{
    for (const FileInfo &info : files)
    {
        if (info.name == file)
        {
            return true;
        }
    }

    return false;
}


bool ListFiles::ExistEqualFile(const FileInfo &rhs) const
{
    for (const FileInfo &info : files)
    {
        if (info.name == rhs.name)
        {
            return (info.size == rhs.size) && (info.crc == rhs.crc);
        }
    }

    return false;
}


bool FileInfo::operator==(const FileInfo &info)
{
    return (name == info.name) && (size == info.size) && (crc == info.crc);
}


void ListFiles::WriteToFile(pchar fileName)
{
    FS::File file;

    file.Create(fileName);

    for (auto &it : files)
    {
        file << it.name.c_str() << " " << (uint64)it.size << " " << (uint64)it.crc << "\r\n";
    }
}


void ListFiles::AppendSubDirectory(pchar directory, std::vector<std::string> *ignoredFiles, std::vector<std::string> *ignoredExtensions, std::vector<std::string> *ignoredDirs)
{

#ifdef WIN32

    std::string mask = std::string(directory) + '/' + '*';

    WIN32_FIND_DATA findData;

    HANDLE handle = FindFirstFileA(mask.c_str(), &findData);

    if (handle == INVALID_HANDLE_VALUE)
    {
        LOG_WRITE("Can't find first file in %s", mask.c_str());
        DISPLAY_LAST_ERROR();
    }
    else
    {
        do
        {
            if (0 == SU::Compare(findData.cFileName, ".") ||
                0 == SU::Compare(findData.cFileName, ".."))
            {
            }
            else
            {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    std::string path = std::string(directory) + '/' + findData.cFileName;

                    bool ignored = false;

                    if (ignoredDirs)
                    {
                        for (uint i = 0; i < ignoredDirs->size(); i++)
                        {
                            if (std::memcmp((std::string(path.c_str() + 2) + '/').c_str(), (*ignoredDirs)[i].c_str(), (*ignoredDirs)[i].size()) == 0)
                            {
                                ignored = true;
                                break;
                            }
                        }
                    }

                    if (!ignored)
                    {
                        AppendSubDirectory(path.c_str(), nullptr, nullptr, ignoredDirs);
                    }
                }
                else
                {
                    std::string fullName;
                    fullName.append(mask.c_str(), mask.size() - 1U);
                    fullName.append(findData.cFileName);

                    AppendFile(fullName, ignoredFiles, ignoredExtensions);
                }
            }

            if (FindNextFileA(handle, &findData) == 0)
            {
                FindClose(handle);
                break;
            }

        } while (true);
    }

#else

    std::string path(&directory[(directory[0] == '/') ? 1 : 0]);

    DIR *dir = opendir(path.c_str());

    if (dir)
    {
        while (true)
        {
            const dirent *dirent = readdir(dir);

            if (!dirent)
            {
                break;
            }

            pchar name = dirent->d_name;

            if (0 == SU::Compare(name, ".") ||
                0 == SU::Compare(name, ".."))
            {
            }
            else
            {
                if (dirent->d_type == DT_DIR || dirent->d_type == DT_LNK)
                {
                    std::string full_path = path + '/' + name;

                    bool ignored = false;

                    if (ignoredDirs)
                    {
                        for (int i = 0; i < ignoredDirs->size(); i++)
                        {
                            if (memcmp((full_path + '/').c_str() + 2, (*ignoredDirs)[i].c_str(), (*ignoredDirs)[i].size()) == 0)
                            {
                                ignored = true;
                                break;
                            }
                        }
                    }

                    if (!ignored)
                    {
                        AppendSubDirectory(full_path.c_str(), nullptr, nullptr, ignoredDirs);
                    }
                }
                else if(dirent->d_type != DT_REG)
                {
                    continue;
                }
                else
                {
                    std::string fullName;
                    fullName.append(path.c_str());
                    fullName.push_back('/');
                    fullName.append(name);

                    AppendFile(fullName, ignoredFiles, ignoredExtensions);
                }
            }
        }

        closedir(dir);
    }

#endif
}


static void ThreadFunction(FileInfo *info, bool *run)
{
    FS::File file;
    file.Open(info->name.c_str(), FS::File::ModeAccess::Read);

    info->size = file.Size();
    info->crc = file.CalculateCheckSum(info->size);

    *run = false;
}


void ListFiles::AppendFile(std::string &fullName, const std::vector<std::string> *ignoredFiles, const std::vector<std::string> *ignoredExtensions)
{
//    fullName.erase(0, 2);

    auto now = std::chrono::steady_clock::now();

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - startBuild);

    if (ignoredExtensions && ExtensionIs(fullName, ignoredExtensions))
    {
        return;
    }

    if (ignoredFiles && FileIs(fullName, ignoredFiles))
    {
        return;
    }

    while (true)
    {
        for(bool &run : runThread)
        {
            if (!run)
            {
                run = true;

                files.emplace_back(FileInfo(fullName.c_str()));

                std::thread(ThreadFunction, &files.back(), &run).detach();

                return;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


bool ListFiles::ExtensionIs(const std::string &fullName, const std::vector<std::string> *ignoredExtensions)
{
    for (const std::string &ext : *ignoredExtensions)
    {
        if (fullName.find(ext) == fullName.size() - ext.size())
        {
            return true;
        }
    }

    return false;
}


bool ListFiles::FileIs(const std::string &fullName, const std::vector<std::string> *ignoredFiles)
{
    for (const std::string &file : *ignoredFiles)
    {
        if (fullName == file)
        {
            return true;
        }
    }

    return false;
}


void ListFiles::Build(pchar directory, std::vector<std::string> *ignoredFiles, std::vector<std::string> *ignoredExtensions, std::vector<std::string> *ignoredDirs)
{
    startBuild = std::chrono::steady_clock::now();

    files.clear();

    std::cout << "\n";

    AppendSubDirectory(directory, ignoredFiles, ignoredExtensions, ignoredDirs);

    while (NumThreads() != 0)
    {
    }
}


int ListFiles::NumThreads()
{
    int result = 0;

    for (int i = 0; i < maxThreads; i++)
    {
        if (runThread[i])
        {
            result++;
        }
    }

    static int counter = 0;

    if (counter++ > 10000 && result < 2)
    {
        counter = 0;                            // \todo Без этого виснет uploader на Linux в release
        LOG_WRITE("%d threads", result);        // и starter.exe на Windows
    }

    return result;
}


bool ListFiles::ExtractFromFile(pchar fileName)
{
    files.clear();

    FS::File file;
    file.Open(fileName, FS::File::ModeAccess::Read);

    std::string string;

    while (file.ReadString(string))
    {
        FileInfo info;

        std::stringstream stream(string);

        stream >> info.name;
        stream >> info.size;
        stream >> info.crc;

        files.push_back(info);
    }

    return true;
}


void ListFiles::ExtractFilesNames(std::vector<std::string> &names)
{
    names.clear();

    for (auto file : files)
    {
        names.push_back(file.name);
    }
}
