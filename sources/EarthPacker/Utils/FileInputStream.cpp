// 2022/09/24 10:28:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/FileInputStream.h"


FileInputStream::FileInputStream(const wxString &file_path) : wxFileInputStream(file_path)
{

}


wxMemoryBuffer FileInputStream::ReadAll()
{
    wxMemoryBuffer result(GetSize());

    wxFileOffset offset = TellI();
    size_t size = GetSize();

    wxFileInputStream::ReadAll(result.GetData(), GetSize());

    return result;
}


wxMemoryBuffer FileInputStream::ReadBytes(int offset, int num_bytes)
{
    if (num_bytes == 0)
    {
        LOG_ERROR("Need read %d bytes", num_bytes);

        return wxMemoryBuffer();
    }

    wxMemoryBuffer result(num_bytes);

    SeekI(offset);

    wxFileInputStream::Read(result.GetData(), num_bytes);

    return result;
}


wxMemoryBuffer FileInputStream::ReadBytes(int num_bytes)
{
    wxMemoryBuffer result(num_bytes);

    wxFileOffset offset = TellI();

    wxFileInputStream::Read(result.GetData(), num_bytes);

    return result;
}


uint FileInputStream::ReadUINT()
{
    uint result = 0;

    wxFileOffset offset = TellI();

    wxFileInputStream::Read(&result, 4);

    return result;
}


int FileInputStream::ReadINT()
{
    wxFileOffset offset = TellI();

    return (int)ReadUINT();
}


float FileInputStream::ReadFloat()
{
    float result = 0.0f;

    wxFileOffset offset = TellI();

    uint bytes = ReadUINT();

    std::memcpy(&result, &bytes, 4);

    return result;
}


uint8 FileInputStream::ReadByte()
{
    wxFileOffset offset = TellI();

    return (uint8)GetC();
}


uint16 FileInputStream::Read2Bytes()
{
    uint16 result = 0;

    wxFileOffset offset = TellI();

    wxFileInputStream::Read(&result, 2);

    return result;
}


uint FileInputStream::ReadUINT(int offset)
{
    uint result = 0;

    SeekI(offset);

    wxFileInputStream::Read(&result, 4);

    return result;
}
