// 2022/09/24 09:03:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/ResourceWD.h"


class GroupWD : public ResourceWD
{
public:
    GroupWD(const String<> &filename, ResourceInfoWD, std::vector<uint8> *data);
};
