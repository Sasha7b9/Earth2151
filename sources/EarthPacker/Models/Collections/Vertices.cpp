// 2022/09/24 19:44:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Collections/Vertices.h"


void Vertices::Create(FileInputStream &stream)
{
    volatile int vertices = (int)stream.ReadUINT();
    vertices = vertices;

    int blocks = (int)stream.ReadUINT();

    for (int i = 0; i < blocks; i++)
    {
        GetVertices(stream.ReadBytes(VERTICES_BLOCK_LENGTH));
    }
}


void Vertices::GetVertices(const wxMemoryBuffer &buffer)
{
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
}


float Vertices::TakeFloat(const void *buffer)
{
    float result = 0.0f;

    std::memcpy(&result, buffer, 4);

    return result;
}
