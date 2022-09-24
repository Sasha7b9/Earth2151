// 2022/09/24 11:33:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Zlib.h"


wxMemoryBuffer Zlib::Decompress(const wxMemoryBuffer &buffer)
{
    wxMemoryInputStream stream(buffer.GetData(), buffer.GetBufSize());

    wxZlibInputStream zstream(stream);

    wxMemoryOutputStream ostream;

    zstream.Read(ostream);

    wxMemoryBuffer result(ostream.GetLength());

    ostream.CopyTo(result.GetData(), ostream.GetLength());

    return result;
}
