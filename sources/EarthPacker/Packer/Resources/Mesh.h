// 2022/09/23 21:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/Resource.h"


class Mesh : public Resource
{
public:
    Mesh(std::string &filename, ResourceInfo, std::vector<uint8> *);
private:
};
