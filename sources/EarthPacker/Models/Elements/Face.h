// 2022/09/24 20:55:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


struct DescriptionModel;


struct Face : public IInputStream
{
    Face(int num);

    int v1 = 0;
    int v2 = 0;
    int v3 = 0;
    int unknown = 0;
};
