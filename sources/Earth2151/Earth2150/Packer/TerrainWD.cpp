// 2022/09/24 09:05:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/TerrainWD.h"


TerrainWD::TerrainWD(const String<> &filename, ResourceInfoWD info, const String<> &translationID, std::vector<uint8> *data) :
    TranslatableResource(filename, info, translationID, data)
{

}
