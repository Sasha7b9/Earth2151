// 2024/09/20 19:37:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/MemoryInputStream.h"
#include <cstring>


MemoryInputStream::MemoryInputStream(const void *_data, int _size) :
    data(nullptr), size(0), pointer(0)
{
    Set(_data, _size);
}


void MemoryInputStream::Set(const void *_data, int _size)
{
    if (data)
    {
        delete[]data;
    }

    if (_data == nullptr || _size == 0)
    {
        data = nullptr;
        size = 0;
        pointer = 0;

        return;
    }

    size = _size;

    data = new uint8[size];

    std::memcpy(data, _data, size);

    pointer = 0;
}


MemoryInputStream::~MemoryInputStream()
{
    delete[]data;
}


void MemoryInputStream::Read(void *buffer, int num_bytes)
{
    std::memcpy(buffer, data + pointer, num_bytes);

    pointer += num_bytes;
}
