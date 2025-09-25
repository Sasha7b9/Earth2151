// 2025/02/26 21:10:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Graphics/CanvasTexture.h"
#include "Utils/Math.h"

#pragma warning(push)
#pragma warning(disable:4201)

/*
*   Текстуры для текстурирования ландшафта
*/


struct TexTile
{
    Point2D _00;    // left up
    Point2D _10;    // rugth up
    Point2D _11;    // right down
    Point2D _01;    // left down

    void Clear()
    {
        _00 = Point2D::zero;
        _10 = Point2D::zero;
        _11 = Point2D::zero;
        _01 = Point2D::zero;
    }
};


// По этому ключу выбираем нужный тайл
union Tile
{
    // Типы текстур  в углах данного тайла
    uint8 tex[4];

    struct _s
    {                //                           +---------+
        uint8 tex00; // 0 - left up               | 0     1 |
        uint8 tex10; // 1 - right up              |         |
        uint8 tex11; // 2 - right down            | 3     2 |
        uint8 tex01; // 3 - left down             +---------+
    };

    _s s;

    bool operator<(const Tile rhs) const
    {
        return M::Hash(tex, sizeof(tex)) < M::Hash(rhs.tex, sizeof(tex));
    }
};


namespace TexLand2150
{
    struct Type
    {
        enum E
        {
            Winter,
            EartlySpring,
            Spring,
            Summer,
            Dessert,
            Volcanic,
            Lava,
            CityRuins
        };
    };

    void SetType(Type::E);

    CanvasTexture *GetCanvas();

    void CreateTiles();

    bool GetTile(const Tile, TexTile &);

    void Destroy();

    // В Moon Project (и в версии 2.8.7.1 EftBP) отличается формат Terrains/*.dat
    bool IsNewFormat();
}


namespace TexTunnels2150
{
    void Init();

    Texture *GetTexture();

    bool GetTile(TexTile &);
}


#pragma warning(pop)
