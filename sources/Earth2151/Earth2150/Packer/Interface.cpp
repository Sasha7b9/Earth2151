// 2022/09/23 22:21:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Packer/Interface.h"


Interface::Interface(const String<> &filename, ResourceInfoWD info, const String<> &translationID) :
    TranslatableResource(filename, info, translationID)
{

}
