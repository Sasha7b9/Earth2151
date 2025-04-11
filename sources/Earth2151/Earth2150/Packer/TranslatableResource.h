// 2022/09/24 09:06:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/ResourceWD.h"


class TranslatableResource : public ResourceWD
{
public:
    TranslatableResource(const String<> &filename, ResourceInfoWD, const String<> &translationID, std::vector<uint8> * = nullptr);
    std::string translationID;
};
