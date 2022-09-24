// 2022/09/23 22:21:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Resources/TranslatableResource.h"


namespace Resources
{
    class Interface : public TranslatableResource
    {
    public:
        Interface(std::string &filename, ResourceInfo, std::string &translationID);
    };
}
