// 2022/09/14 14:35:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/FileSystem.h"
#include "Utils/SystemUtils.h"
#include "Utils/StringUtils.h"
#ifdef WIN32
#else
    #include "Base/C4PrefixLinux.h"
#endif
#include <cstring>


uint FS::File::CalculateCheckSum(size_t size)
{
    uint checksum = 0;

    const int sizeBuffer = 512 * 1024;

    char buffer[sizeBuffer] = { 0 };

    while (size > 0)
    {
        size_t readBytes = (size >= sizeBuffer) ? sizeBuffer : size;
        size -= readBytes;

        Read(buffer, readBytes);

        for (size_t i = 0; i < readBytes; i++)
        {
            checksum = (uint8)buffer[i] + (checksum << 6U) + (checksum << 16U) - checksum;
        }
    }

    return checksum;
}


void FS::RemoveFile(pchar nameFile)
{
#if PiWINDOWS

    if (DeleteFileA(nameFile) == 0)
    {
        DWORD error = GetLastError();
        if (error != 2 &&               // Не удаётся найти указанный файл
            error != 3)                 // Системе не удается найти указанный путь
        {
//            LOG_ERROR("Can't remove file %s", nameFile.c_str());

            DISPLAY_LAST_ERROR();
        }
    };

#else

    if (remove(nameFile) != 0)
    {
        if (errno != 2)                 // Файл не существует
        {
            LOG_ERROR_HI("Can't remove file %s", nameFile);

            DISPLAY_LAST_ERROR();
        }
    }

#endif
}


bool FS::RemoveDirectory(pchar name)
{
#if PiWINDOWS

    SHFILEOPSTRUCTA str;
    str.hwnd = NULL;
    str.wFunc = FO_DELETE;

    char buffer[512];
    std::strcpy(buffer, name);
    buffer[std::strlen(name)] = 0;
    buffer[std::strlen(name) + 1] = 0;
    buffer[std::strlen(name) + 2] = 0;
    str.pFrom = buffer;
    str.pTo = 0;
    str.fFlags = FOF_NOCONFIRMATION + FOF_SILENT;
    str.fAnyOperationsAborted = false;
    str.hNameMappings = nullptr;
    str.lpszProgressTitle = nullptr;

    int result = SHFileOperation(&str);                              // Удаляем всё из каталога

    if (result != 0)
    {
        LOG_ERROR_HI("Error SHFileOperation for directory \"%s\" with code %d", name, result);
    }

    return result == 0;

#else

    LOG_ERROR_HI("FS::RemoveDirectory() Not implemented for Linux");

    return false;

#endif
}


bool FS::File::Open(pchar _name, ModeAccess::E mode)
{
    name = _name;

#if PiWINDOWS

    DWORD access = 0;

    if (mode & ModeAccess::Read)
    {
        access |= GENERIC_READ;
    }
    if (mode & ModeAccess::Write)
    {
        access |= GENERIC_WRITE;
    }

    handle = CreateFileA(_name, access, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE)
    {
        LOG_ERROR_HI("Can't open file \"%s\"", name.c_str());

        DISPLAY_LAST_ERROR();

        handle = nullptr;

        return false;
    }

    return true;

#else

    uint access = 0;

    if (mode == ModeAccess::Read)       { access = O_RDONLY; } 
    else if (mode == ModeAccess::Write) { access = O_RDWR; }
    else                                { access = O_RDWR;   }

    fileDesc = open(_name, access, 0);

    if (fileDesc == -1)
    {
        LOG_ERROR_HI("Can't open file \"%s\"", name.c_str());

        DISPLAY_LAST_ERROR();

        fileDesc = -1;

        return false;
    }

    return true;

#endif
}


bool FS::File::Create(pchar _name, ModeAccess::E mode)
{
    name = _name;

    FS::RemoveFile(_name);

#if PiWINDOWS

    DWORD access = 0;

    if (mode & ModeAccess::Read)
    {
        access |= GENERIC_READ;
    }
    if (mode & ModeAccess::Write)
    {
        access |= GENERIC_WRITE;
    }

    handle = CreateFileA(_name, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (GetLastError() == 3)            // Системе не удаётся найти указанный путь
    {
        std::string fullName(_name);

        size_t pos = fullName.rfind('/');

        if (pos != std::string::npos)
        {
            std::string path = fullName.substr(0, pos);

            FS::CreateDirectory(path.c_str());

            handle = CreateFileA(_name, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        }
    }

    if (handle == INVALID_HANDLE_VALUE)
    {
        DISPLAY_LAST_ERROR();

        handle = nullptr;

        return false;
    }

    return true;

#else

    uint access = 0;

    if (mode == ModeAccess::Read) { access = O_RDONLY; }
    else if (mode == ModeAccess::Write) { access = O_WRONLY; }
    else { access = O_RDWR; }

    fileDesc = open(_name, access | O_CREAT, S_IWRITE | S_IREAD);

    if (fileDesc < 0)
    {
        LOG_ERROR_HI("Can't open file %s", name);

        DISPLAY_LAST_ERROR();

        fileDesc = -1;

        return false;
    }

    return true;

#endif
}


void FS::File::Write(const void *buffer, int numBytes)
{
#if PiWINDOWS

    DWORD counter = 0;

    if (WriteFile(handle, buffer, (DWORD)numBytes, &counter, NULL) == 0)
    {
        LOG_ERROR_HI("Can't write to file %s", name.c_str());

        DISPLAY_LAST_ERROR();
    }

    FlushFileBuffers(handle);

#else

    if (write(fileDesc, buffer, numBytes) != numBytes)
    {
        LOG_ERROR_HI("Can't write to file %s", name.c_str());

        DISPLAY_LAST_ERROR();
    }

    fsync(fileDesc);

#endif
}


void FS::File::Read(std::string &string, size_t size)
{
    string.resize(size);

    Read(&string[0], size);
}



void FS::File::Read(void *buffer, size_t numBytes)
{
#if PiWINDOWS

    DWORD counter = 0;

    if (ReadFile(handle, buffer, (DWORD)numBytes, &counter, NULL) == 0)
    {
        LOG_ERROR_HI("Can't read from file %s %llu bytes", name.c_str(), numBytes);

        DISPLAY_LAST_ERROR();
    }

#else

    if (read(fileDesc, buffer, numBytes) != (ssize_t)numBytes)
    {
        LOG_ERROR_HI("Can't read from file %s %d bytes", name.c_str(), numBytes);

        DISPLAY_LAST_ERROR();
    }

#endif
}


bool FS::File::ReadString(std::string &string)
{
    string.clear();

#if PiWINDOWS

    DWORD counter = 0;

    bool Ox0D = false;
    bool Ox0A = false;

    do 
    {
        char symbol = 0;
        ReadFile(handle, &symbol, 1, &counter, NULL);

        if (counter != 0)
        {
            string.append(&symbol, 1);
        }

        if (symbol == 0x0D) { Ox0D = true; }
        if (symbol == 0x0A) { Ox0A = true; }

    } while (counter == 1 && (!Ox0D || !Ox0A));

    return Ox0D && Ox0A;

#else

    size_t size = Size();

    size_t start = file_position;

    bool end_string = false;                // Если true, то встретили символ конца строки

    while (file_position < size)
    {
        char symbol = 0;

        lseek(fileDesc, file_position, SEEK_SET);

        if (read(fileDesc, &symbol, 1) >= 0)
        {
            file_position++;

            if (symbol == 0x0d || symbol == 0x0a)
            {
                end_string = true;
            }
            else
            {
                if (end_string)
                {
                    file_position--;
                    lseek(fileDesc, file_position, SEEK_SET);
                    break;
                }
            }
        }
        else
        {
            return false;
        }
    }

    int num_bytes = file_position - start;

    string.resize(num_bytes + 1);

    lseek(fileDesc, start, SEEK_SET);

    read(fileDesc, string.data(), num_bytes);

    string.data()[num_bytes] = '\0';

    return true;

#endif
}


bool FS::CreateDirectory(pchar path)
{
#if PiWINDOWS

#define CP_UTF8                   65001       // UTF-8 translation

    char buffer[1024];

    GetCurrentDirectoryA(1024, buffer);

    std::string full(buffer);

    full.append("\\");
    full += path;

    int size = MultiByteToWideChar(CP_UTF8, 0, &full[0], (int)full.size(), NULL, 0);
    std::wstring wpath((uint64)size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &full[0], (int)full.size(), &wpath[0], size);

    if (SHCreateDirectory(NULL, wpath.c_str()) != 0)
    {
        LOG_ERROR_HI("Can't create directory %s", full.c_str());

        DISPLAY_LAST_ERROR();

        return false;
    }

    return true;

#else

    LOG_ERROR_HI("Not implemented for Linux");

    return false;

#endif
}


namespace FS
{
    File &operator<< (File &out, pchar buffer)
    {
        out.Write(buffer, (int)std::strlen(buffer));

        return out;
    }

    File &operator<< (File &out, uint64 value)
    {
        char buffer[30];
        sprintf(buffer, "%llu", value);

        out.Write(buffer, (int)std::strlen(buffer));

        return out;
    }
}


size_t FS::File::Size()
{
#if PiWINDOWS

#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)

    DWORD hiSize = 0;

    DWORD result = GetFileSize(handle, &hiSize);

    if (result == INVALID_FILE_SIZE)
    {
        DISPLAY_LAST_ERROR();

        return 0;
    }

    return (size_t)result;

#else

    long pos = lseek(fileDesc, 0L, SEEK_SET);

    long size = lseek(fileDesc, 0L, SEEK_END);

    lseek(fileDesc, pos, SEEK_SET);

    return size;

#endif
}


void FS::File::Close()
{
#if PiWINDOWS

    if (handle != nullptr)
    {
        CloseHandle(handle);
        handle = nullptr;
    }

#else

    if (fileDesc != -1)
    {
        close(fileDesc);
        fileDesc = -1;
    }

#endif
}


bool FS::File::IsOpened()
{
#if PiWINDOWS

    return (handle != nullptr);

#else

    return (fileDesc != -1);

#endif
}
