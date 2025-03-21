// 2022/09/24 09:04:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Packer/ResourceWD.h"


class LevelWD : public ResourceWD
{
public:
    LevelWD(const String<> &filename, ResourceInfoWD, const String<> &id, std::vector<uint8> *);
private:
    std::string id;
};
