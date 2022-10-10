// 2022/09/24 20:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


class TextureInfo : public IInputStream
{
public:
    void Create();

    std::string file_name;
};
