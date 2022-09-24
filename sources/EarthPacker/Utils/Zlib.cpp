// 2022/09/24 11:33:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Zlib.h"


wxMemoryBuffer Zlib::Decompress(const wxMemoryBuffer &buffer)
{
    wxMemoryInputStream stream(buffer.GetData(), buffer.GetBufSize());

    wxZlibInputStream zstream(stream);

    wxMemoryBuffer result(1);

    while (!zstream.Eof())
    {
        uint8 byte = 0;
        zstream.Read(&byte, 1);

        result.AppendByte((char)byte);
    }

    return result;
}
