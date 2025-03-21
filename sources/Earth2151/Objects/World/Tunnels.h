// 2025/03/15 11:36:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"


namespace Tunnels
{
    void ReadTunnels(HeapBuffer &, File &file, int width, int height);

    void Destroy();
}
