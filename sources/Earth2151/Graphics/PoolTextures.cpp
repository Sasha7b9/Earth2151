// 2021/12/14 9:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Graphics/PoolTextures.h"
#include "Utils/Math.h"


namespace PoolTextures
{
    static Array<CanvasTexture *> textures;

    static CanvasTexture *CreateLandscape();
    static CanvasTexture *CreateSelect();

    static bool is_destructed = false;
}


void PoolTextures::Construct()
{
    textures.SetArrayElementCount((int)CanvasTexture::Type::Count);

    textures[(int)CanvasTexture::Type::Landscape] = CreateLandscape();
    textures[(int)CanvasTexture::Type::Select] = CreateSelect();

    is_destructed = true;
}


void PoolTextures::Destruct()
{
    if (is_destructed)
    {
        return;
    }

    for(int i = 0; i < textures.GetArrayElementCount(); i++)
    {
        delete textures[i];
    }
}
 

Texture *PoolTextures::Get(CanvasTexture::Type type)
{
    return textures[(uint)type]->GetTexture();
}


CanvasTexture *PoolTextures::CreateLandscape()
{
    static const int SIZE = 64;

    CanvasTexture canvas(SIZE, SIZE);

    canvas.BeginPaint({{0.0f, 0.3f, 0.0f}, 1.0f}, {0.0f, 0.0f, 0.3f});
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            canvas.SetColorBrush({{Random::RandomFloat(0.0f, 0.15f),
                                   Random::RandomFloat(0.3f, 0.45f),
                                   Random::RandomFloat(0.0f, 0.15f)},  1.0f});
            canvas.SetPixel(i, j);
        }
    }
    canvas.SetColorBrush({0.0f, 0.3f, 0.1f});
    canvas.DrawRectangle(2, 2, SIZE - 3, SIZE - 3);
    canvas.EndPaint();

    CanvasTexture *result = new CanvasTexture(SIZE, SIZE);

    result->BeginPaint(Color::white, Color::white);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int ix[4] = {0, 1, 0, -1};
            int iy[4] = {-1, 0, 1, 0};

            ColorRGBA full = M::UINTtoColor(*canvas.Pixel(i, j));

            if(i != 0 && i != 31 && j != 0 && j != 31)
            {
                for (int ii = 0; ii < 4; ii++)
                {
                    uint colorPixel = *canvas.Pixel(i + ix[ii], j + iy[ii]);
                    ColorRGBA color = M::UINTtoColor(colorPixel);
                    full += color;
                }

                full /= 5;
            }

            result->SetColorBrush(full);
            result->SetPixel(i, j);
        }
    }

    result->SetColorBrush(Color::green);

    result->FillRegion(5, 5, 10, 10);

    result->EndPaint();

    return result;
}


CanvasTexture *PoolTextures::CreateSelect()
{
    CanvasTexture *canvas = new CanvasTexture(32, 32);

    canvas->BeginPaint({{0.0f, 0.0f, 0.0f}, 0.0f}, {0.25, 0.25, 0.25});
    canvas->DrawRectangle(0, 0, 32, 32);
    canvas->DrawLine(0, 0, 31, 31);
    canvas->DrawLine(31, 0, 0, 31);
    canvas->EndPaint();

    return canvas;
}
