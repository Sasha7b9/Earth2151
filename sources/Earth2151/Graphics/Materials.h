// 2024/06/05 21:06:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TypeMaterial
{
    enum E
    {
        Diffuse,
        Count
    };
};


namespace Materials
{
    MaterialObject *Create(TypeMaterial::E, const ColorRGBA &);
}
