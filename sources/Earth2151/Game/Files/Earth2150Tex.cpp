// 2025/02/25 16:52:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Earth2150Tool.h"
#include "Utils/Math.h"


bool Earth2150::Reader::ReadTex(FileTex2150 &info)
{
    File file;

    if (file.OpenFile(info.file_name_tex, kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR_HI("Can not read file \"%s\"", info.file_name_tex.c_str());

        return false;
    }

    FileReader reader{ &file };

    Texture2150 texture;

    if (texture.Read(reader))
    {
        if (texture.sub_type.IsCollection() ||
            texture.sub_type.IsGrouped() ||
            texture.sub_type.IsSides())
        {
            if (texture.group_count)
            {
                const int group_count = texture.group_count;

                for (int i = 0; i < group_count; i++)
                {
                    Texture2150 tex;

                    info.textures.AppendArrayElement(tex);

                    info.textures[info.textures.GetArrayElementCount() - 1].Read(reader);
                }
            }
            else
            {
                const int count = texture.count;

                for (int i = 0; i < count; i++)
                {
                    Texture2150 tex;

                    info.textures.AppendArrayElement(tex);

                    info.textures[info.textures.GetArrayElementCount() - 1].Read(reader);
                }
            }
        }
        else
        {
            Texture2150 text2;
            info.textures.AppendArrayElement(text2);
            reader.Reset();
            info.textures[0].Read(reader);
        }

        texture.Destroy();
    }

    if (!reader.IsEOF())
    {
        LOG_WRITE("Input stream for texture not empty. Left %d bytes", reader.BytesLeft());
    }

    return true;
}


bool Texture2150::Read(FileReader &reader)
{
    //                                                                                  8 байт типа файла
    const uint8 type_file_mask[8] = { 0x54, 0x45, 0x58, 0x00, 0x01, 0x00, 0x00, 0x00 };

    uint8 type_file[8];

    reader.ReadBuffer(type_file, 8);

    if (std::memcmp(type_file, type_file_mask, sizeof(type_file_mask)) != 0)
    {
        return false;
    }

    //                                                                                  Заголовок файла
    type.value = reader.ReadByte();

    animation_type = reader.ReadByte();

    reserved = reader.ReadByte();

    sub_type.value = reader.ReadByte();

    if ((sub_type.IsUnknownFlag1() ||       // Unknown flag 1
        sub_type.IsUnknownFlag2()) &&       // Unknown flag 2
        !sub_type.IsCollection())            // Collection
    {
        unknown1 = reader.ReadByte();
        unknown2 = reader.ReadByte();
        reserved2 = reader.ReadByte();
        reserved3 = reader.ReadByte();
    }

    if (sub_type.IsCollection() ||          // Collection
        sub_type.IsSides())                 // Sides
    {
        count = reader.ReadInt();
    }

    if (sub_type.IsGrouped())               // Grouped
    {
        group_count = reader.ReadInt();
    }

    if (type.IsTexture() &&           // Texture
        !sub_type.IsCollection())         // Collection
    {
        width = reader.ReadInt();
        height = reader.ReadInt();

        if (type.IsLod())           // Lod
        {
            lod_levels = reader.ReadInt();
        }
    }

    if (sub_type.IsCollection() ||
        sub_type.IsGrouped() ||
        sub_type.IsSides())
    {
        return true;
    }
    else
    {
        int w = width;
        int h = height;

        int counter = 0;

        do
        {
            counter++;

            if (w == width && h == height)
            {
                image = new Image2150(w, h);
                canvas = new CanvasTexture(w, h);

                canvas->BeginPaint(Color::white, Color::white);

                ColorRGBA *colors = image->storage;

                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        uint col = 0;

                        reader.ReadBuffer(&col, 4);

                        uint8 r = (uint8)(col);         // \todo по порядку чтения цветов есть вопросы
                        col >>= 8;                      // rgb и grb почти не отличаются. При grb оттеники некоторых поверхностей ландшафта краснят
                        uint8 g = (uint8)(col);         // В оставшихся 4-х вариантах чтения изображение имеет сине-малиновые оттении
                        col >>= 8;
                        uint8 b = (uint8)(col);
                        col >>= 8;
                        uint8 alpha = (uint8)(col);

                        ColorRGBA color((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)alpha / 255.0f);

                        *colors = color;

                        canvas->SetColorBrush(color);

                        canvas->SetPixel(x, y);

                        colors++;
                    }
                }

                canvas->EndPaint();
            }
            else
            {
                uint8 buffer[1024 * 104];

                reader.ReadBuffer(buffer, w * h * 4);
            }

            w /= 2;
            h /= 2;

        } while (counter < lod_levels);
    }

    return true;
}


static uint ColToU(ColorRGBA color)
{
    uint red = (uint)(color.red * 255);
    uint green = (uint)(color.green * 255);
    uint blue = (uint)(color.blue * 255);
    uint alpha = (uint)(color.alpha * 255);

    return blue | (green << 8) | (red << 16) | (alpha << 24);
}
