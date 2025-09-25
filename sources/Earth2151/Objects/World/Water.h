// 2025/03/16 12:33:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"


class Water
{
public:

    Water(HeapBuffer &, File &file, int width, int height);

    ~Water();

    // Уровень моря
    float LevelSea()
    {
        return level_sea;
    }

    bool GetLevel(const Point2D &coord, float *);

    // Возвращает минимальынй уровень воды
    float GetLevelMin();

    bool IsExist()
    {
        return is_exist;
    }

private:

    float level_sea = 0.0f;

    void CreateGeometry();

    struct WaterCell
    {
        int x;
        int y;
        float height;
    };

    Array<WaterCell> cells;

    struct WaterHeight
    {
        float value;
        bool valid;
    };

    Array<Array<WaterHeight>> waters;

    float level_min = 1e3f;

    bool is_exist = false;
};
