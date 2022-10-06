// 2022/10/02 20:38:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/DescriptionFile.h"


bool DescriptionFileWD::IsValid() const
{
    return false;
}


void DescriptionFileWD::AppendLine(const DescInfoWD &info)
{
    push_back(info);
}


int DescriptionFileWD::Size()
{
    return (int)size();
}
