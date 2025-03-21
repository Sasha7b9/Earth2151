// 2022/09/23 20:34:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/ResourceWD.h"


ResourceWD::ResourceWD(const String<> &filename, ResourceInfoWD _info, std::vector<uint8> *_unknown_data) :
    file_name(filename), info(_info)
{
    if (_unknown_data)
    {
        unknown_data = *_unknown_data;
    }
}
