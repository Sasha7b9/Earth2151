// 2025/02/23 11:34:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template<int size_buffer>
struct StackBuffer
{
    void Append(char symbol)
    {
        Append(&symbol, 1);
    }

    void Append(void *buffer, int size)
    {
        if (pointer + size <= size_buffer)
        {
            std::memcpy(data + pointer, buffer, size);
            pointer += size;
        }
        else
        {
            LOG_ERROR_HI("Small write buffer");
        }
    }

    uint8 data[size_buffer];
    int pointer = 0;
};
