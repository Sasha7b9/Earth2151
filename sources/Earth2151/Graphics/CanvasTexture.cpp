// 2021/12/13 16:29:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Graphics/CanvasTexture.h"
#include "Utils/Math.h"


CanvasTexture::CanvasTexture(int width, int height)
{
    header = new TextureHeader();

    header->textureType = kTextureType2D;
    header->textureFlags =  0;
    header->colorSemantic = kTextureSemanticDiffuse;
    header->alphaSemantic = kTextureSemanticTransparency;
    header->textureFormat = kTextureFormatGammaRGBA;
    header->textureWidth = width; //-V525
    header->textureHeight = height;
    header->textureDepth = 1;
    header->wrapMode[0] = kTextureWrapRepeat;
    header->wrapMode[1] = kTextureWrapRepeat;
    header->wrapMode[2] = kTextureWrapRepeat;
    header->mipmapCount = 1;
    header->mipmapDataOffset = 0;
    header->auxiliaryDataSize = 0;
    header->auxiliaryDataOffset = 0;

    if (width * height == 4096)
    {
        width = width;
    }

    data = new uint[(size_t)(width * height)]; //-V1028

    texture = Texture::GetTexture(header, data);
}


CanvasTexture::~CanvasTexture()
{
    texture->Release();
    delete header;
    delete []data;
}


void CanvasTexture::SetColorBackground(const ColorRGBA &color)
{
    colorBackground = M::ColorToUINT(color);
}


void CanvasTexture::SetColorBrush(const ColorRGBA &color)
{
    colorBrush = M::ColorToUINT(color);
}


void CanvasTexture::SetPixel(int x, int y)
{
    *Pixel(x, y) = colorBrush;
}


void CanvasTexture::SetPixel(int x, int y, uint8 r, uint8 g, uint8 b, uint8 a)
{
    uint *address = Pixel(x, y);
    *address = (uint)(r | (g << 8) | (b << 16) | (a << 24));
}


void CanvasTexture::DrawLine(int x1, int y1, int x2, int y2)
{
    if (y1 == y2)
    {
        M::PutInOrder(x1, x2);
        int width = x2 - x1 + 1;

        uint *pixel = Pixel(x1, y1);

        while (width--)
        {
            *pixel++ = colorBrush;
        }
    }
    else if (x1 == x2)
    {
        M::PutInOrder(y1, y2);
        int height = y2 - y1 + 1;

        uint *pixel = Pixel(x1, y1);

        while (height--)
        {
            *pixel = colorBrush;
            pixel += header->textureWidth; //-V102
        }
    }
    else
    {
        int x0 = x1;
        x1 = x2;
        int y0 = y1;
        y1 = y2;

        int x = x0;
        int y = y0;
        int dx = (int)Fabs((float)(x1 - x0));
        int dy = (int)Fabs((float)(y1 - y0));
        int s1 = (int)Fsgn((float)(x1 - x0));
        int s2 = (int)Fsgn((float)(y1 - y0));
        int temp = 0;
        bool exchange = false;
        if (dy > dx)
        {
            temp = dx;
            dx = dy;
            dy = temp;
            exchange = true;
        }
        else
        {
            exchange = false;
        }
        int e = 2 * dy - dx;
        for (int i = 0; i <= dx; i++)
        {
            SetPixel(x, y);
            while (e >= 0)
            {
                if (exchange)
                {
                    x += s1;
                }
                else
                {
                    y += s2;
                }
                e -= 2 * dx;
            }
            if (exchange)
            {
                y += s2;
            }
            else
            {
                x += s1;
            }
            e = e + 2 * dy;
        }
    }
}


void CanvasTexture::FillRegion(int x0, int y0, int width, int height)
{
    for (int y = y0; y < y0 + height - 1; y++)
    {
        DrawLine(x0, y, x0 + width - 1, y);
    }
}


void CanvasTexture::DrawRectangle(int x1, int y1, int width, int height)
{
    int x2 = x1 + width - 1;
    int y2 = y1 + height - 1;

    DrawLine(x1, y1, x2, y1);
    DrawLine(x1, y2, x2, y2);

    DrawLine(x1, y1, x1, y2);
    DrawLine(x2, y1, x2, y2);
}


void CanvasTexture::EndPaint()
{
    texture->UpdateRect({0, 0, header->textureWidth, header->textureHeight}, 0, data);
}


void CanvasTexture::BeginPaint(const ColorRGBA &background, const ColorRGBA &brush)
{
    SetColorBackground(background);
    SetColorBrush(brush);
    Clear();
}


void CanvasTexture::Clear()
{
    int num_pixels = header->textureHeight * header->textureWidth;

    for (int i = 0; i < num_pixels; i++)
    {
        data[i] = colorBackground;
    }
}


uint *CanvasTexture::Pixel(int x, int y)
{
    const int offset = y * header->textureWidth + x;

    if (offset < 0 || offset >= header->textureHeight * header->textureWidth)
    {
        static uint empty;
        return &empty;
    }

    return data + offset;
}


static uint ConvertPixel(uint pixel)
{
    ColorRGBA color = M::UINTtoColor(pixel);

    float red = color.red;
    float blue = color.blue;

    color.red = blue;
    color.blue = red;

    return M::ColorToUINT(color);
}


void CanvasTexture::SaveToBMP(pchar file_name)
{
    int width = header->textureWidth;
    int height = header->textureHeight;

    const int SIZE_BUFFER = width * height;

    uint *buffer = new uint[SIZE_BUFFER];

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            *buffer++ = ConvertPixel(*Pixel(x, y));
        }
    }

    buffer -= SIZE_BUFFER;

    BITMAPFILEHEADER bmFH =
    {
        0x4d42,
        sizeof(BITMAPFILEHEADER) + 40 + 1024 + SIZE_BUFFER * sizeof(buffer[0]),
        0,
        0,
        sizeof(BITMAPFILEHEADER) + 40 + 1024
    };

    BITMAPINFOHEADER bmIH =
    {
        sizeof(BITMAPINFOHEADER),   // size;
        width,                      // width;
        height,                     // height;
        1,                          // planes;
        32,                         // bitCount;
        0,                          // compression;
        0,                          // sizeImage;
        0,                          // xPelsPerMeter;
        0,                          // yPelsPerMeter;
        0,                          // clrUsed;
        0                           // clrImportant;
    };

    File out;

    if (out.OpenFile(file_name, kFileCreate) != kFileOkay)
    {
        LOG_ERROR("Can not create file %s", file_name);

        delete[]buffer;

        return;
    }

    out.WriteFile(&bmFH, sizeof(bmFH));

    out.WriteFile(&bmIH, sizeof(bmIH));

    uint8 zero_buffer[1024];

    std::memset(zero_buffer, 0, 1024);

    out.WriteFile(zero_buffer, 1024);

    uint *pointer = buffer + SIZE_BUFFER - width;

    uint *out_buf = new uint[width * sizeof(uint)];

    while (pointer >= buffer)
    {
        std::memcpy(out_buf, pointer, width * sizeof(uint));

        out.WriteFile(out_buf, width * sizeof(uint));

        pointer -= width;
    }

    out.CloseFile();

    delete []out_buf;
    delete []buffer;
}
