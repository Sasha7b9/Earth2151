// 2022/09/23 20:34:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/Resource.h"


using namespace Packer;


Resource::Resource(std::string &filename, ResourceInfo _info, std::vector<uint8> *_unknown_data) :
    file_name(filename), info(_info)
{
    if (_unknown_data)
    {
        unknown_data = *_unknown_data;
    }
}
