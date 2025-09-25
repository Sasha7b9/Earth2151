// 2025/02/26 21:11:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Files/TexLand2150.h"
#include "Game/Earth2150Tool.h"
#include "Objects/World/Landscape.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#include "Earth2151.h"
#include "Game/Files/TexMesh2150.h"
#include <map>


namespace TexLand2150
{
    struct Storage
    {
        void Append(const Tile key_tile, const TexTile &tex)
        {
            auto it = tiles.find(key_tile);

            if (it == tiles.end())
            {
                tiles.try_emplace(key_tile , new Array<TexTile> ());
            }

            it = tiles.find(key_tile);

            Array<TexTile> *tex_tiles = it->second;

            for (int i = 0; i < tex_tiles->GetArrayElementCount(); i++)
            {
                if (std::memcmp(&(*tex_tiles)[i], &tex, sizeof(tex)) == 0)
                {
                    return;
                }
            }

            tex_tiles->AppendArrayElement(tex);
        }

        bool Get(const Tile tile, TexTile &tex)
        {
            auto it = tiles.find(tile);

            if (it != tiles.end())
            {
                tex = (*it->second)[std::rand() % it->second->GetArrayElementCount()];

                return true;
            }

            return false;
        }

        int GetNumElements()
        {
            return (int)tiles.size();
        }

        void Destroy()
        {
            for (auto &item : tiles)
            {
                item.second->PurgeArray();
                delete item.second;
            }

            tiles.clear();
        }

        // Возвращает наиболее подобный тайл - где совпадает максимальное количество уголов
        void GetSimilar(const Tile tile, TexTile &tex)
        {
            int number_mutches = 0;
            Tile key;

            for (auto item : tiles)
            {
                int count = 0;

                for (int i = 0; i < 4; i++)
                {
                    if (tile.tex[i] == item.first.tex[i])
                    {
                        count++;
                    }
                }

                if (count > number_mutches)
                {
                    key = item.first;
                    number_mutches = count;
                }
            }

            Get(key, tex);
        }

    private:

        std::map<Tile, Array<TexTile> * > tiles;
    };

    static Storage storage_tiles;


    static FileTex2150 texWinter;
    static FileTex2150 texEarlySpring;
    static FileTex2150 texSpring;
    static FileTex2150 texSummer;
    static FileTex2150 texDessert;
    static FileTex2150 texVolcanic;
    static FileTex2150 texLava;
    static FileTex2150 texCityRuins;

    static FileTex2150 *texs[] =
    {
        &texWinter,
        &texEarlySpring,
        &texSpring,
        &texSummer,
        &texDessert,
        &texVolcanic,
        &texLava,
        &texCityRuins,
        nullptr
    };

    // После выхода The Moon Project изменили формат ландшафта. Также новый формат в версии 2.8.7.1 EftBP
    static bool is_new_format = false;

    static String<> name_file_dat;                  // Файл *.dat, в котором хранится информация о тайлинге

    static int num_tiles_in_dat = 512 - 128 - 20;

    static CanvasTexture *canvas = nullptr;

    static bool is_init = false;

    static void Init();

    static bool BuildCanvas(FileTex2150 *);

    static void GetTexTile(int num_tex, TexTile &);

    // Получает тайл из текстуры
    static void GetTexTile(int x, int y, TexTile &);
}


void TexLand2150::SetType(Type::E type)
{
//    LOG_WRITE("Set type texture %d", (int)type);

    if (!is_init)
    {
        texWinter.file_name_tex = RESOURCE_PATH("Terrains/TEREN1.TEX");
        texEarlySpring.file_name_tex = RESOURCE_PATH("Terrains/TEREN2.TEX");
        texSpring.file_name_tex = RESOURCE_PATH("Terrains/TEREN3.TEX");
        texSummer.file_name_tex = RESOURCE_PATH("Terrains/TEREN4.TEX");
        texDessert.file_name_tex = RESOURCE_PATH("Terrains/TEREN5.TEX");
        texVolcanic.file_name_tex = RESOURCE_PATH("Terrains/TEREN6.TEX");
        texLava.file_name_tex = RESOURCE_PATH("Terrains/TEREN7.TEX");
        texCityRuins.file_name_tex = RESOURCE_PATH("Terrains/TEREN8.TEX");

        is_init = true;
    }

    if (!texs[type]->canvas)
    {
        name_file_dat = RESOURCE_PATH("Terrains/TEREN1.dat");

        File file;

        file.OpenFile(name_file_dat, kFileReadOnly);

        uint8 byte = 0;

        file.ReadFile(&byte, 1);

        is_new_format = (byte & 0xF) != 0;      // Это означает, что игра не Earth 2150 (хотя в версии 2.8.7.1 тоже такой формат), а Moon Project или Lost Souls

        if (is_new_format)                
        {
            char types[8] = { '3', '2', '1', '4', '5', '6', '7', '8' };              // Terrains.dat

            name_file_dat[name_file_dat.GetStringLength() - 5] = types[type];
        }
        else
        {
            name_file_dat[name_file_dat.GetStringLength() - 5] = (char)(0x30 + (int)type + 1);
        }

        Earth2150::Reader::ReadTex(*texs[type]);

        BuildCanvas(texs[type]);
    }

    canvas = texs[type]->canvas;
}


void TexLand2150::Destroy()
{
    for (int i = 0; true; i++)
    {
        if (texs[i] == nullptr)
        {
            break;
        }

        texs[i]->Destroy();
    }

    storage_tiles.Destroy();
}


CanvasTexture *TexLand2150::GetCanvas()
{
    return canvas;
}


void TexLand2150::Init()
{
    for (int i = 0; texs[i]; i++)
    {
        if (Earth2150::Reader::ReadTex(*texs[i]))
        {
            if (BuildCanvas(texs[i]))
            {
//                texs[i]->canvas->SaveToBMP(texs[i]->file_name_tex + ".bmp");
            }
        }
    }
}


bool TexLand2150::BuildCanvas(FileTex2150 *tex)
{
    if (tex->textures.GetArrayElementCount() != 512)
    {
        LOG_ERROR_HI("Error texture file : %d textures instead of 512", tex->textures.GetArrayElementCount());

        return false;
    }

    // В текстуре 8196 x 8196 помещается 16384 тайлов
    //     -//-   4096 x 4096    -//-     4096
    //     -//-   2048 x 2048    -//-     1024
    CanvasTexture *canv = new CanvasTexture(2048, 2048);

    canv->BeginPaint(Color::white, Color::white);

    for (int counter = 0; counter < 512; counter++)
    {
        Image2150 *image = tex->textures[counter].image;

        ColorRGBA *color = image->storage;

        if (color->alpha != 1.0f)
        {
            LOG_ERROR_HI("In texture pixel alpha non zero");
        }

        int start_y = (counter * 64) % 1024;
        int start_x = (counter * 64) / 1024 * 64;

        for (int x = 0; x < 64; x++)
        {
            for (int y = 0; y < 64; y++)
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


void TexLand2150::GetTexTile(int y, int x, TexTile &tex)
{
    static const float size_tile = 1.0f / 32.0f;

    float fy = (float)y * size_tile;
    float fx = (float)x * size_tile;

    tex._00.x = fx;
    tex._00.y = fy;

    tex._10.x = fx;
    tex._10.y = fy + size_tile; //-V525

    tex._11.x = fx + size_tile;
    tex._11.y = fy + size_tile;

    tex._01.x = fx + size_tile;
    tex._01.y = fy;
}


void TexLand2150::GetTexTile(int num_tex, TexTile &tile)
{
    GetTexTile(std::rand() % 8, num_tex & 0x07, tile);
}


bool TexLand2150::GetTile(const Tile tile, TexTile &tex)
{
    bool result = false;

    tex.Clear();

    if (storage_tiles.Get(tile, tex))
    {
        result = true;
    }
    else
    {
        Tile t = tile;

        M::Swap(t.s.tex00, t.s.tex01);                  // Меняем местами лево и право
        M::Swap(t.s.tex10, t.s.tex11);

        if (storage_tiles.Get(t, tex))
        {
            M::Swap(tex._00, tex._01);
            M::Swap(tex._10, tex._11);

            result = true;
        }
        else
        {
            t = tile;

            M::Swap(t.s.tex00, t.s.tex10);              // Меняем местами верх и низ
            M::Swap(t.s.tex01, t.s.tex11);

            if (storage_tiles.Get(t, tex))
            {
                M::Swap(tex._00, tex._10);
                M::Swap(tex._01, tex._11);

                result = true;
            }
            else
            {
                t = tile;

                M::Swap(t.s.tex00, t.s.tex11);              // Перекидываем по диагоналям
                M::Swap(t.s.tex01, t.s.tex10);

                if (storage_tiles.Get(t, tex))
                {
                    M::Swap(tex._00, tex._11);
                    M::Swap(tex._01, tex._10);

                    result = true;
                }
            }
        }
    }

    if (!result)
    {
        static bool first = true;

        if (first)
        {
            first = false;
            LOG_ERROR_LOW("Not tile for cell %d %d %d %d", tile.tex[0], tile.tex[1], tile.tex[2], tile.tex[3]);
        }

        storage_tiles.GetSimilar(tile, tex);

        result = true;
    }

    return result;
}


void TexLand2150::CreateTiles()
{
    Point2D size = LANDSCAPE->GetSize();

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            Tile tile = LANDSCAPE->GetTile(x, y);

            uint8 texture = tile.s.tex00;

            if (texture == tile.s.tex10 && texture == tile.s.tex11 && texture == tile.s.tex01)
            {
                TexTile tex;

                GetTexTile(texture, tex);

                storage_tiles.Append(tile, tex);
            }
        }
    }

    File file;

    if (file.OpenFile(name_file_dat.c_str(), kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR_HI("Can not open file %s", file.GetFileName());
    }

    int pos_begin_tiles = 0x101;

    if (is_new_format)
    {
        char buffer[64];

        file.ReadFile(buffer, 4);

        uint8 byte = 0;                   // Длина строки

        file.ReadFile(&byte, 1);

        pos_begin_tiles += 3 + byte + 22;

        if (name_file_dat[name_file_dat.GetStringLength() - 5] == '8')
        {
            num_tiles_in_dat -= 28;
        }
    }

    file.SetFilePosition(pos_begin_tiles);

    FileReader reader(&file);

    int x = 0;
    int y = 8;

    // Файл содержит 512 структур, но 20 из них пустые, а 128 - "основные"
    for (int i = 0; i < num_tiles_in_dat; i++)
    {
        Tile tile;

        // Читаем углы тайла
        {
            uint16 data;

            reader.ReadBuffer(&data, 2);

            tile.s.tex00 = data & 0x07;
            data >>= 3;
            tile.s.tex10 = data & 0x07;
            data >>= 3;
            tile.s.tex11 = data & 0x07;
            data >>= 3;
            tile.s.tex01 = data & 0x07;
        }

        TexTile tex;

        GetTexTile(x, y, tex);

        storage_tiles.Append(tile, tex);

        // Перекидываем через одну из диагоналей, получая "зеркальный" тайл
        {
            M::Swap(tile.s.tex10, tile.s.tex01);
            M::Swap(tex._00, tex._11);

            storage_tiles.Append(tile, tex);
        }

        x++;
        if (x == 16)
        {
            x = 0;
            y++;
        }
    }
}


void TexTunnels2150::Init()
{
}


Texture *TexTunnels2150::GetTexture()
{
    return TexMesh2150::GetTexture("Textures/TunnelWall.tex", 60);
}


bool TexTunnels2150::GetTile(TexTile &tile)
{
    float size = 1.0f / 1.0f;

    tile._00 = Point2D{ 0.0f, 0.0f };
    tile._10 = Point2D{ size, 0.0f };
    tile._11 = Point2D{ size, size };
    tile._01 = Point2D{ 0.0f, size };

    return true;
}


bool TexLand2150::IsNewFormat()
{
    return is_new_format;
}
