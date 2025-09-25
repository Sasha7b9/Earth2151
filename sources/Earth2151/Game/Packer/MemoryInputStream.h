// 2024/09/20 19:28:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class MemoryInputStream
{
public:

    MemoryInputStream(const void *_data = nullptr, int _size = 0);

    ~MemoryInputStream();

    void Set(const void *, int);

    bool Eof() const
    {
        return pointer >= size;
    }

    void Read(void *buffer, int size);

    // Возвращает текущее значение указателя
    int TellI() const
    {
        return pointer;
    }

    // Принудительно устанавливает значение указателя
    void SeekI(int i)
    {
        pointer = i;
    }

private:

    uint8 *data = nullptr;
    int size = 0;
    int pointer = 0;
};
