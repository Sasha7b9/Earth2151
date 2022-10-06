// 2022/09/24 09:04:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Packer/Resources/Resource.h"


class Level : public Resource
{
public:
    Level(std::string &filename, ResourceInfo, std::string &id, std::vector<uint8> *);
private:
    std::string id;
};
