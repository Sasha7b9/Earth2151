// 2025/03/16 12:33:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"


namespace Water
{
    void Create(HeapBuffer &, File &file, int width, int height);

    void Destroy();

    // Уровень моря
    float LevelSea();

    bool GetLevel(const Point2D &coord, float *);

    // Возвращает минимальынй уровень воды
    float GetLevelMin();

    bool IsExist();
}
