// 2025/03/01 13:42:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace TexMesh2150
{
    void Init();

    void DeInit();

    Texture *GetTexture(pchar name, int num = 0);

    int NumTextures(pchar name);
}
