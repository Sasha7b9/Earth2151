// 2025/02/12 22:16:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Utils/StringUtils.h"


String<> SU::Vec3toStr(const Vector3D &vec)
{
    return Text::Format("%f:%f:%f", (float)vec.x, (float)vec.y, (float)vec.z);
}


FileReader::FileReader(File *file)
{
    size = file->GetFileSize();

    if (size == 56)
    {
        size = size;
    }

    data = new uint8[size];

    pointer = file->GetFilePosition();

    file->SetFilePosition(0);

    file->ReadFile(data, size);

    file->SetFilePosition(pointer);
}


uint8 FileReader::ReadByte()
{
    uint8 result = 0;

    ReadBuffer(&result, 1);

    return result;
}


int16 FileReader::ReadInt16()
{
    int16 result = 0;

    ReadBuffer(&result, 2);

    return result;
}


int FileReader::ReadInt()
{
    int result = 0;

    ReadBuffer(&result, 4);

    return result;
}


uint FileReader::ReadUInt()
{
    uint result = 0;

    ReadBuffer(&result, sizeof(result));

    return result;
}


float FileReader::ReadFloat()
{
    float result = 0.0f;

    ReadBuffer(&result, sizeof(result));

    return result;
}


bool FileReader::ReadBool()
{
    return (ReadByte() != 0);
}


uint8 *FileReader::ReadBuffer(void *buffer, int num_bytes)
{
    std::memcpy(buffer, data + pointer, num_bytes);

    pointer += num_bytes;

    return (uint8 *)buffer;
}


String<> FileReader::ReadString(int size_length)
{
    uint size = ReadUInt();

    char buffer[1024];

    ReadBuffer(buffer, size);

    buffer[size] = '\0';

    return String<>(buffer);
}


bool FileReader::IsEOF()
{
    return pointer >= size;
}


void FileReader::Reset()
{
    pointer = 0;
}


bool SU::EndsWith(pchar str, pchar substr)
{
    if (std::strlen(substr) >= std::strlen(str))
    {
        return false;
    }

    pchar pointer = str + std::strlen(str) - std::strlen(substr);

    return std::strcmp(substr, pointer) == 0;
}


bool SU::BeginWith(pchar str, pchar substr)
{
    if (std::strlen(substr) >= std::strlen(str))
    {
        return false;
    }

    return std::memcmp(str, substr, std::strlen(substr)) == 0;
}


void SU::SplitToWords(pchar input, Array<String<>> &out, pchar delim)
{
    std::string line(input);

    char *token = std::strtok(line.data(), delim);

    while (token != nullptr)
    {
        out.AppendArrayElement(token);
        token = std::strtok(nullptr, delim);
    }
}


double SU::ASCIItoFloat(pchar str)
{
    return std::atof(str);
}
