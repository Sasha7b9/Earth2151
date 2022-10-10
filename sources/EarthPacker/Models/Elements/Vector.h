// 2022/09/24 16:39:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


struct DescriptionModel;


struct Vector : public IInputStream
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}

    Vector(pchar name);

    // Конструктор из 12-ти байт
    Vector(uint8 *);

    void Create(pchar name);

    string ToString() const;
};
