// 2022/09/23 21:52:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/PackMesh.h"


using namespace Packer;


Mesh::Mesh(std::string &filename, ResourceInfo info, std::vector<uint8> *data) : Resource(filename, info, data)
{

}
