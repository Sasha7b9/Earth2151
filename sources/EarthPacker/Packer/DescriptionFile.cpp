// 2022/10/02 20:38:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/DescriptionFile.h"


bool DescriptionFile::IsValid() const
{
    return false;
}


void DescriptionFile::AppendLine(pchar line)
{
    push_back(line);
}
