// 2022/09/24 09:04:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/LevelWD.h"


LevelWD::LevelWD(const String<> &filename, ResourceInfoWD info, const String<> &_id, std::vector<uint8> *data) :
    ResourceWD(filename, info, data), id(_id)
{

}
