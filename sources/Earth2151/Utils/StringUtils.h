// 2025/02/12 22:17:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct FileReader
{
    FileReader(File *_file);
    ~FileReader()
    {
        delete[]data;
    }

    uint8 ReadByte();
    int ReadInt();
    uint8 *ReadBuffer(void *buffer, int size);
    // Параметр - ширина поля длины строки, которая начинается с первого байта
    String<> ReadString();
    int16 ReadInt16();
    uint ReadUInt();
    float ReadFloat();
    bool ReadBool();
    bool IsEOF();
    void Reset();
    int BytesLeft() const;
    const uint8 *GetData() const
    {
        return data + pointer;
    }

    uint8 *data;
    uint size = 0;
    int pointer = 0;
};


namespace SU
{
    String<> Vec3toStr(const Vector3D &);

    bool EndsWith(pchar, pchar);

    bool BeginWith(pchar, pchar);

    void SplitToWords(pchar, Array<String<>> &, pchar delim);

    double ASCIItoFloat(pchar);

    int Compare(pchar str1, pchar str2);

    // Удалить открывающую и закрывающую кавычки, ежели таковые имеются
    String<> RemoveQuotes(pchar);

    int FindSymbol(pchar, char);

    String<> GetSubstring(pchar, int start, int end);
}
