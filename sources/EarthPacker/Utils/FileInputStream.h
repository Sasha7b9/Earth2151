// 2022/09/24 10:26:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class FileInputStream : public wxFileInputStream
{
public:
    FileInputStream(const wxString &file_path);

    static void Set(FileInputStream *stream) { current = stream; };
    static FileInputStream *Get() { return current; };

    wxMemoryBuffer ReadAll();
    wxMemoryBuffer ReadBytes(int num_bytes);
    wxMemoryBuffer ReadBytes(int offset, int num_bytes);
    uint8 ReadByte();
    uint16 Read2Bytes();
    uint ReadUINT();
    int ReadINT();
    uint ReadUINT(int offset);
    float ReadFloat();
private:

    static FileInputStream *current;
};


class IInputStream
{
protected:
    uint ReadUINT(pchar name);
};
