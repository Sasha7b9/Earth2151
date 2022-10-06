// 2022/09/24 09:04:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Level.h"


Level::Level(std::string &filename, ResourceInfo info, std::string &_id, std::vector<uint8> *data) :
    Resource(filename, info, data), id(_id)
{

}
