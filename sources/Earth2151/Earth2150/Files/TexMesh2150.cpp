// 2025/03/01 13:42:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Files/TexMesh2150.h"
#include "Earth2150/Earth2150Tool.h"
#include "Clock.h"
#include "Earth2151.h"
#include <map>


namespace TexMesh2150
{
    static bool is_init = false;

    static std::map<String<>, FileTex2150 *> textures;

    static bool BuildCanvas(FileTex2150 *);
}


Texture *TexMesh2150::GetTexture(pchar name, int num)
{
    char file[512];
    std::strcpy(file, name);
    char *pointer = file + std::strlen(file);
    while (*pointer != '\\' && *pointer != '/')
    {
        pointer--;
    }
    pointer++;

    for (uint i = 0; i < std::strlen(pointer); i++)
    {
        pointer[i] = (char)toupper(pointer[i]);
    }

    if (textures.find(String<>(pointer)) == textures.end())
    {
        return nullptr;
    }

    CanvasTexture *canvas = textures[String<>(pointer)]->textures[num].canvas;

    return canvas ? canvas->GetTexture() : nullptr;
}


int TexMesh2150::NumTextures(pchar name)
{
    char file[512];
    std::strcpy(file, name);
    char *pointer = file + std::strlen(file);
    while (*pointer != '\\' && *pointer != '/')
    {
        pointer--;
    }
    pointer++;

    for (uint i = 0; i < std::strlen(pointer); i++)
    {
        pointer[i] = (char)toupper(pointer[i]);
    }

    if (textures.find(String<>(pointer)) == textures.end())
    {
        return 1;
    }

    return textures[String<>(pointer)]->textures.GetArrayElementCount();

}


void TexMesh2150::Init()
{
    if (is_init)
    {
        return;
    }

    uint start_time = UCOUNT_MS;

    is_init = true;

    Map<FileReference> file_map;

    FileMgr::BuildFileMap(RESOURCE_PATH("Textures/"), &file_map, [](pchar name, uint flags, const void *)
    {
        return SU::EndsWith(name, ".tex") || SU::EndsWith(name, ".TEX");
    });

    int num_images = 0;

    uint time_read = 0;

    if (!file_map.Empty())
    {
        FileReference *element = file_map.GetFirstMapElement();

        while (element)
        {
            FileTex2150 *texture = new FileTex2150();

            texture->file_name_tex = RESOURCE_PATH((String<>("Textures/") + element->fileName).c_str());

            uint start_read = UCOUNT_MS;

            Earth2150::Reader::ReadTex(*texture);

            time_read += UCOUNT_MS - start_read;

            char name[512];
            std::strcpy(name, texture->file_name_tex.c_str());

            char *pointer = name + std::strlen(name);

            while (*pointer != '\\' && *pointer != '/')
            {
                pointer--;
            }
            pointer++;

            for (uint i = 0; i < std::strlen(pointer); i++)
            {
                pointer[i] = (char)toupper(pointer[i]);
            }

            textures[String<>(pointer)] = texture;

//          texture->canvas->SaveToBMP(texture->file_name_tex + ".bmp");

            num_images++;

            element = element->GetNextMapElement();
        }
    }

    LOG_WRITE("Time create %d textures %f s, %f ms/image", num_images, (UCOUNT_MS - start_time) / 1e3f, (float)(UCOUNT_MS - start_time) / file_map.GetMapElementCount());
}


bool TexMesh2150::BuildCanvas(FileTex2150 *tex)
{
    if (tex->textures.GetArrayElementCount() == 0)
    {
        return false;
    }

    int size_canvas = 0;

    int size_image = tex->textures[0].width;

    const int num_textures = tex->textures.GetArrayElementCount();

    if (num_textures == 1)
    {
        size_canvas = size_image;
    }
    else if (num_textures < 5)
    {
        size_canvas = size_image * 2;
    }
    else if (num_textures < 17)
    {
        size_canvas = size_image * 4;
    }
    else if (num_textures < 65)
    {
        size_canvas = size_image * 8;
    }

    CanvasTexture *canv = new CanvasTexture(size_canvas, size_canvas);

    canv->BeginPaint(Color::white, Color::white);

    for (int num_texture = 0; num_texture < num_textures; num_texture++)
    {
        Image2150 *image = tex->textures[num_texture].image;

        ColorRGBA *color = image->storage;

        int start_x = (num_texture * size_image) % size_canvas;
        int start_y = (num_texture * size_image) / size_canvas * size_image;

        for (int y = 0; y < size_image; y++)
        {
            for (int x = 0; x < size_image; x++)
            {
                canv->SetColorBrush(*color);

                canv->SetPixel(start_x + x, start_y + y);

                color++;
            }
        }
    }

    canv->EndPaint();

    tex->canvas = canv;

    return true;
}


void TexMesh2150::DeInit()
{
    for (auto tex : textures)
    {
        delete tex.second;
    }

    textures.clear();

    is_init = false;
}
