// 2022/09/24 09:02:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Group.h"


using namespace Resources;


Group::Group(std::string &filename, ResourceInfo info, std::vector<uint8> *data) :
    Resource(filename, info, data)
{

}
