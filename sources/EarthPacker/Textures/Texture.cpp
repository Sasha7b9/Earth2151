// 2022/11/26 15:41:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Textures/Texture.h"


Texture::Texture(const wxFileName &_file_name) : file_name(_file_name)
{
    FileInputStream main_stream(file_name.GetFullPath());

    IInputStream::SetInputStream(&main_stream);

    TextureHeader header = ReadHeader();
}


void Texture::SaveAsBMP()
{

}


TextureHeader Texture::ReadHeader()
{
    uint8 buffer[16];

    stream->Read(buffer, 16);

    if (buffer[0] != 84 && buffer[1] != 69 && buffer[2] != 88)
    {
        LOG_ERROR("Not texture format");
    }

    int number_maps = 0;
    if (buffer[11] == 128 || buffer[11] == 67 || buffer[11] == 16)
    {
        std::memcpy(&number_maps, &buffer[12], 4);
    }

    if (buffer[11] == 192)
    {
        uint8 temp_buffer[4];
        stream->Read(temp_buffer, 4);

        std::memcpy(&number_maps, &buffer[12], 4);

        int two_maps = 0;
        std::memcpy(&two_maps, temp_buffer, 4);

        number_maps *= two_maps;
    }

    return TextureHeader(buffer[8], buffer[10], number_maps);
}
