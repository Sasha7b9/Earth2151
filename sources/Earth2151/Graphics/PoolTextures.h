// 2021/12/14 9:17:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Graphics/CanvasTexture.h"


namespace PoolTextures
{
    void Construct();
    void Destruct();

    Texture *Get(CanvasTexture::Type type);
}
