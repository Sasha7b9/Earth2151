// 2022/09/24 09:06:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/ResourceWD.h"


class TranslatableResource : public ResourceWD
{
public:

    TranslatableResource(const String<> &filename, ResourceInfoWD info, const String<> &_translationID, std::vector<uint8> *unknown_data = nullptr) :
        ResourceWD(filename, info, unknown_data), translationID(_translationID)
    {

    }

    std::string translationID;
};
