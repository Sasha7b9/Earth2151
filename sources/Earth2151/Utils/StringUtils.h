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
    String<> ReadString(int size_length);
    int16 ReadInt16();
    uint ReadUInt();
    float ReadFloat();
    bool ReadBool();
    bool IsEOF();
    void Reset();

    int GetPointer() const
    {
        return pointer;
    }

//private:

    uint8 *data;
    uint size = 0;
    uint pointer = 0;
};


namespace SU
{
    String<> Vec3toStr(const Vector3D &);

    bool EndsWith(pchar, pchar);

    bool BeginWith(pchar, pchar);

    void SplitToWords(pchar, Array<String<>> &, pchar delim);
}
