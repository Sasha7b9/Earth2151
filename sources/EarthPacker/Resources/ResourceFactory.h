// 2022/09/23 20:33:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Resources/Resource.h"


namespace Resources
{
    class ResourceFactory
    {
    public:
        static Resource Create(wxInputStream &);
    };
}
