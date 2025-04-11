// 2022/09/14 14:35:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace FS
{
    void RemoveFile(pchar);

    bool RemoveDirectory(pchar);

    bool CreateDirectory(pchar);

#ifdef DATA_CATALOG
    // Возвращает полный путь к файлу из каталога DataCatalog
    inline std::string DataFile(pchar name)
    {
        return std::string(TheResourceMgr->DataCatalog()->GetRootPath()) + DATA_CATALOG + name;
    }
#endif


    class File
    {
    public:

        struct ModeAccess
        {
            enum E
            {
                Read    = 1 << 0,
                Write   = 1 << 1
            };
        };

        ~File() { Close(); };

        // Открывает существующий файл
        bool Open(pchar name, ModeAccess::E mode);

        // Создаёт новый файл. Если файл с таким именем уже существует, стирает его содержимое
        bool Create(pchar name, ModeAccess::E mode = (ModeAccess::E)(ModeAccess::Read | ModeAccess::Write));

        uint CalculateCheckSum(size_t size);

        void Write(const void *buffer, int size);

        void Read(void *buffer, size_t size);

        void Read(std::string &string, size_t size);

        bool ReadString(std::string &string);

        size_t Size();

        void Close();

        bool IsOpened();

        friend File &operator<< (File &out, pchar );
        friend File &operator<< (File &out, uint64);

    private:

        std::string name;

        size_t file_position = 0;

#ifdef WIN32
        HANDLE handle = nullptr;
#else
        int fileDesc = -1;
#endif
    };
}
