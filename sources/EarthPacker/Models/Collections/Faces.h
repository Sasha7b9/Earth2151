// 2022/09/24 20:55:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Face.h"


struct DescriptionModel;


struct Faces : std::list<Face>, public IInputStream
{
    void Create();
};
