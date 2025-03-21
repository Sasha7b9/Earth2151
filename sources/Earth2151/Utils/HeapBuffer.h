// 2025/02/23 11:51:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct HeapBuffer
{
    HeapBuffer(int _size_chunk) : size_chunk(_size_chunk)
    {
        IncreaseSize(size_chunk);
    }

    ~HeapBuffer()
    {
        Destroy();
    }

    void Destroy()
    {
        delete[]data;
    }

    void WriteData(void *buffer, int size_buffer)
    {
        if (pointer + size_buffer > size)
        {
            IncreaseSize(size_chunk);
        }

        std::memcpy(data + pointer, buffer, size_buffer);
        pointer += size_buffer;
    }

    bool ReadData(void *buffer, int num_bytes)
    {
        if (pointer_read + num_bytes <= pointer)
        {
            std::memcpy(buffer, data + pointer_read, num_bytes);
            pointer_read += num_bytes;

            return true;
        }

        return false;
    }

    void IncreaseSize(int append_size)
    {
        if (data)
        {
            size += append_size;
            uint8 *data_new = new uint8[size];
            std::memcpy(data_new, data, pointer);
            delete[] data;
            data = data_new;

        }
        else
        {
            data = new uint8[append_size];
            size = append_size;
        }
    }

    void ResetWrite()
    {
        pointer = 0;
    }

    void ResetRead()
    {
        pointer_read = 0;
    }

    bool FullyReaded() const
    {
        return pointer_read == pointer;
    }

    uint8 *data = nullptr;

    int pointer = 0;
    int pointer_read = 0;
    int size = 0;
    const int size_chunk;
};
