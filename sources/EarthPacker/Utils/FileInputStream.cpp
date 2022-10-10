// 2022/09/24 10:28:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/FileInputStream.h"
#include "Models/Model.h"


FileInputStream *IInputStream::stream = nullptr;
DescriptionModel *IInputStream::desc = nullptr;


FileInputStream::FileInputStream(const wxString &file_path) : wxFileInputStream(file_path)
{

}


wxMemoryBuffer FileInputStream::ReadAll()
{
    wxMemoryBuffer result(GetSize());

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

    wxFileInputStream::Read(result.GetData(), num_bytes);

    return result;
}


uint FileInputStream::ReadUINT()
{
    uint result = 0;

    wxFileInputStream::Read(&result, 4);

    return result;
}


int FileInputStream::ReadINT()
{
    return (int)ReadUINT();
}


float FileInputStream::ReadFloat()
{
    float result = 0.0f;

    uint bytes = ReadUINT();

    std::memcpy(&result, &bytes, 4);

    return result;
}


uint8 FileInputStream::ReadByte()
{
    return (uint8)GetC();
}


uint16 FileInputStream::Read2Bytes()
{
    uint16 result = 0;

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


uint IInputStream::ReadUINT(pchar name)
{
    InfoModel info(InfoModel::Type::UINT, stream->TellI(), name);

    uint result = stream->ReadUINT();

    desc->AppendInfo(info.AppendBytes(result));

    return result;
}


uint16 IInputStream::ReadUINT16(pchar name)
{
    InfoModel info(InfoModel::Type::UINT16, stream->TellI(), name);

    uint16 result = stream->Read2Bytes();

    desc->AppendInfo(info.AppendBytes(&result, 2));

    return result;
}


uint8 IInputStream::ReadByte(pchar name)
{
    InfoModel info(InfoModel::Type::Byte, stream->TellI(), name);

    uint8 result = stream->ReadByte();

    desc->AppendInfo(info.AppendByte(result));

    return result;
}
