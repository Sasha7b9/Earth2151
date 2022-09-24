// 2022/09/24 09:02:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/Group.h"


Packer::Group::Group(std::string &filename, ResourceInfo info, std::vector<uint8> *data) :
    Resource(filename, info, data)
{

}
