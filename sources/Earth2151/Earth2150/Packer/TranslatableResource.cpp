// 2022/09/24 09:06:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/TranslatableResource.h"


TranslatableResource::TranslatableResource(const String<> &filename, ResourceInfoWD info, const String<> _translationID, std::vector<uint8> *unknown_data) :
    ResourceWD(filename, info, unknown_data), translationID(_translationID)
{

}
