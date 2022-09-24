// 2022/09/24 09:05:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Terrain.h"


using namespace Resources;


Terrain::Terrain(std::string &filename, ResourceInfo info, std::string &translationID, std::vector<uint8> *data) :
    TranslatableResource(filename, info, translationID, data)
{

}
