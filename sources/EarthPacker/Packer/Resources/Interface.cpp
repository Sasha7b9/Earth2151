// 2022/09/23 22:21:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/Interface.h"


Interface::Interface(std::string &filename, ResourceInfo info, std::string &translationID) :
    TranslatableResource(filename, info, translationID)
{

}
