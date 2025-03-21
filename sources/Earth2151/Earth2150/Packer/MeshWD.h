// 2022/09/23 21:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/ResourceWD.h"


class MeshWD : public ResourceWD
{
public:
    MeshWD(const String<> &filename, ResourceInfoWD, std::vector<uint8> *);
private:
};
