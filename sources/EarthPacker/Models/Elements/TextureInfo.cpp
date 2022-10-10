// 2022/09/24 20:01:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/TextureInfo.h"


void TextureInfo::Create()
{
    int fileNameLenght = (int)ReadUINT("Len file name");

    ReadText("Name texture", fileNameLenght);

//    file_name = (char *)buffer.GetData();
}
