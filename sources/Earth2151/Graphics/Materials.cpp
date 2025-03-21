// 2024/06/05 21:07:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Graphics/Materials.h"


MaterialObject *Materials::Create(TypeMaterial::E type, const ColorRGBA &color)
{
    MaterialObject *material = new MaterialObject();

    if (type == TypeMaterial::Diffuse)
    {
        material->AddAttribute(new DiffuseAttribute(color));
    }

    return material;
}
