// 2022/09/24 09:06:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/TranslatableResource.h"


class TerrainWD : public TranslatableResource
{
public:
    TerrainWD(const String<> &filename, ResourceInfoWD, const String<> &translationID, std::vector<uint8> *data);
};
