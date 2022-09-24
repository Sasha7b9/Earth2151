// 2022/09/24 20:01:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/TextureInfo.h"


void TextureInfo::Create(FileInputStream &stream)
{
    int fileNameLenght = (int)stream.ReadUINT();

    wxMemoryBuffer buffer = stream.ReadBytes(fileNameLenght);

    ((char *)buffer.GetData())[buffer.GetBufSize()] = 0;

    file_name = (char *)buffer.GetData();

    file_name = file_name; 
}
