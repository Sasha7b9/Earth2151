// 2024/09/20 16:10:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class MemoryBufferWD
{
public:
    MemoryBufferWD(uint _size = 0) : size(_size) { }

    void Set(void *_buffer, int _size)
    {
        size = _size;

        if (size > MAX_SIZE)
        {
            LOG_ERROR_HI("Very big size : %d", _size);

            size = MAX_SIZE;
        }

        MemoryMgr::CopyMemory(_buffer, data, size);
    }

    void *GetData() { return data; };
    uint GetSize() const  { return size; }
    const uint8 &operator[](int i) const
    {
        if (i >= (int)size)
        {
            LOG_ERROR_HI("Index out of range");
        }

        return data[i];
    }
private:
    static const int MAX_SIZE = 2 * 1024 * 1024;
    uint8 data[MAX_SIZE];
    uint size;
};
