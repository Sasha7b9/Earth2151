// 2022/09/23 21:52:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Mesh.h"


using namespace Resources;


Mesh::Mesh(std::string &filename, ResourceInfo info, wxMemoryBuffer *data) : Resource(filename, info, data)
{

}
