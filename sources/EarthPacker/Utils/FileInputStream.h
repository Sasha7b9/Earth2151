// 2022/09/24 10:26:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DescriptionModel;


class FileInputStream : public wxFileInputStream
{
public:
    FileInputStream(const wxString &file_path);

    wxMemoryBuffer ReadAll();
    wxMemoryBuffer ReadBytes(int num_bytes);
    wxMemoryBuffer ReadBytes(int offset, int num_bytes);
    uint8 ReadByte();
    uint16 Read2Bytes();
    uint ReadUINT();
    int ReadINT();
    uint ReadUINT(int offset);
    float ReadFloat();
};


class IInputStream
{
public:
    static void SetInputStream(FileInputStream *_stream) { stream = _stream; }
    static void SetDescriptionModel(DescriptionModel *_desc) { desc = _desc; };
protected:
    uint ReadUINT(pchar name);
    uint8 ReadByte(pchar name);
    uint16 ReadUINT16(pchar name);
    static FileInputStream *stream;
    static DescriptionModel *desc;
};
