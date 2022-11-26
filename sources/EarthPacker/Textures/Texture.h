// 2022/11/26 15:41:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


struct TextureHeader
{
    TextureHeader(int _type, int _sub_type, int _number_of_maps)
        : type(_type), sub_type(_sub_type), number_of_maps(_number_of_maps) {}

    int type;
    int sub_type;
    int number_of_maps;
};


class Texture : public IInputStream
{
public:
    Texture(const wxFileName &);

    void SaveAsBMP();

private:
    wxFileName file_name;

    TextureHeader ReadHeader();
};
