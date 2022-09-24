// 2022/09/24 17:54:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Light.h"
#include "Utils/FileInputStream.h"


class Lights : public std::list<Light>
{
public:
    Lights(FileInputStream &);
private:
    static const int NUMBER_OF_LIGHTS = 5;
};
