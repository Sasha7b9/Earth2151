// 2022/09/23 22:21:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/TranslatableResource.h"


class Interface : public TranslatableResource
{
public:

    Interface(const String<> &filename, ResourceInfoWD info, const String<> &translationID) :
        TranslatableResource(filename, info, translationID)
    {
    }
};
