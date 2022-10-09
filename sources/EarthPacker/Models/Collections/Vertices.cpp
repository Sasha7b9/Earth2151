// 2022/09/24 19:44:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Vertices.h"
#include "Models/Model.h"


void Vertices::Create(FileInputStream &stream, DescriptionModel &desc)
{
    InfoModel info(InfoModel::Type::Vertices, stream.TellI(), "Vertices");

    ReadUINT("num vertices");

    uint blocks = ReadUINT("num blocks");

    for (uint i = 0; i < blocks; i++)
    {
        GetVertices(i);
    }

    info.size = (int)stream.TellI() - info.header.offset;

    desc.AppendInfo(info);
}


uint Vertices::ReadUINT(pchar name)
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::UINT, stream.TellI(), name);

    uint result = stream.ReadUINT();

    DescriptionModel::Get()->AppendInfo(info.AppendBytes(result));

    return result;
}


void Vertices::GetVertices(int num_block)
{
    FileInputStream &stream = *FileInputStream::Get();

    InfoModel info(InfoModel::Type::VerticesBlock, stream.TellI(), wxString::Format("block %d", num_block));

    wxMemoryBuffer buffer = stream.ReadBytes(VERTICES_BLOCK_LENGTH);

    uint8 *data = (uint8 *)buffer.GetData();

    for (int i = 0; i < VERTICES_IN_BLOCK; i++)
    {
        int idx = i * FIELD_SIZE;

        float x = TakeFloat(data + idx + 0x00);
        float y = -TakeFloat(data + idx + 0x10);
        float z = TakeFloat(data + idx + 0x20);

        float normalX = TakeFloat(data + idx + 0x30);
        float normalY = -TakeFloat(data + idx + 0x40);
        float normalZ = TakeFloat(data + idx + 0x50);

        float u = TakeFloat(data + idx + 0x60);
        float v = 1 - TakeFloat(data + idx + 0x70);

        push_back(Vertex(Vector(x, y, z), Vector(normalX, normalY, normalZ), u, v));
    }

    DescriptionModel::Get()->AppendInfo(info.AppendBytes(buffer));
}


float Vertices::TakeFloat(const void *buffer)
{
    float result = 0.0f;

    std::memcpy(&result, buffer, 4);

    return result;
}
