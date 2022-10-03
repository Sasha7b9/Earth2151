// 2022/09/24 11:33:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Zlib.h"


wxMemoryBuffer Zlib::Decompress(const wxMemoryBuffer &buffer)
{
    static size_t full_size = 0;

    size_t size = buffer.GetBufSize();

    full_size += size;

    LOG_WRITE("Decompress %d bytes, all %f MB", size, (float)full_size / 1024.0f / 1024.0f);

    wxMemoryInputStream stream(buffer.GetData(), size);

    wxMemoryOutputStream ostream;

    wxZlibInputStream(stream).Read(ostream);

    wxMemoryBuffer result((size_t)ostream.GetLength());

    ostream.CopyTo(result.GetData(), (size_t)ostream.GetLength());

    return result;
}
