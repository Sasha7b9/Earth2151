// 2022/09/24 09:06:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/TranslatableResource.h"


using namespace Packer;


TranslatableResource::TranslatableResource(std::string &filename, ResourceInfo info, std::string _translationID, std::vector<uint8> *unknown_data) :
    Resource(filename, info, unknown_data), translationID(_translationID)
{

}
