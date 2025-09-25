// 2022/09/24 09:04:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Packer/ResourceWD.h"


class LevelWD : public ResourceWD
{
public:

    LevelWD(const String<> &filename, ResourceInfoWD info, const String<> &_id, std::vector<uint8> *data) :
        ResourceWD(filename, info, data), id(_id)
    {

    }

private:

    std::string id;
};
