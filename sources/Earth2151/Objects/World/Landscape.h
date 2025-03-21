// 2025/02/23 13:20:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Earth2150Tool.h"
#include "Earth2150/Files/TexLand2150.h"


// Переводит координату y из координт файла .lnd в наши координаты
#define Y_FROM_LND(y)   Landscape::YfromLND(y)


namespace Landscape
{
    void Create(Level2150 &);

    void Destroy();

    Point2D GetSize();

    int GetNumColumns();

    int GetNumRows();

    float GetHeight(const Point2D &, Bivector3D *normal = nullptr);

    float GetHeightApproximately(const Point2D &);

    Tile GetTile(int x, int y);

    // Переводит координату y из координт файла .lnd в наши координаты
    int YfromLND(int);

    float MaxHeight();
    float MinHeight();

    bool IsCreated();
}
