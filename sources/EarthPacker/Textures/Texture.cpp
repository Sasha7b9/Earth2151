// 2022/11/26 15:41:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Textures/Texture.h"


const bool Texture::high_resolution_only = true;


Texture::Texture(const wxFileName &_file_path) : file_path(_file_path)
{
    wxString output_path = file_path.GetPath();

    FileInputStream main_stream(file_path.GetFullPath());

    IInputStream::SetInputStream(&main_stream);

    const TextureHeader header = ReadHeader();

    if (header.number_of_maps > 0)
    {
        for (int i = 0; i < header.number_of_maps; i++)
        {
            const TextureHeader t = ReadHeader();

            vector<Image> images = ReadBitmap(t.type, t.sub_type);

            if (high_resolution_only)
            {
                images[0].SaveAsBMP(output_path.c_str().AsChar(), file_path.GetName().c_str().AsChar(), i);
            }
            else
            {
                for (const Image &image : images)
                {
                    image.SaveAsBMP(output_path.c_str().AsChar(), file_path.GetName().c_str().AsChar(), i);
                }
            }
        }
    }
    else
    {
        vector<Image> images = ReadBitmap(header.type, header.sub_type);

        if (high_resolution_only)
        {
            images[0].SaveAsBMP(output_path.c_str().AsChar(), file_path.GetName().c_str().AsChar(), 0);
        }
        else
        {
            for (const Image &image : images)
            {
                image.SaveAsBMP(output_path.c_str().AsChar(), file_path.GetName().c_str().AsChar(), 0);
            }
        }
    }
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


vector<Image> Texture::ReadBitmap(int type, int sub_type)
{
    int info_length = 0;
    switch (type)
    {
    case 6:
    case 38:
        info_length = 12;
        break;
    case 34:
        info_length = sub_type > 0 ? 24 : 8;
        break;
    default:
        info_length = 8;
        break;
    }

    vector<Image> images;

    vector<uint8> dimensions(info_length);

    stream->Read(dimensions.data(), info_length);

    int width = 0;
    memcpy(&width, dimensions.data(), 4);

    int height = 0;
    memcpy(&height, dimensions.data() + 4, 4);

    uint number_mipmaps = 1;

    if (type == 38 || type == 6)
    {
        memcpy(&number_mipmaps, dimensions.data() + 8, 4);
    }

    do
    {
        Image image(width, height);

        for (int w = 0; w < width; w++)
        {
            for (int h = 0; h < height; h++)
            {
                uint8 red = stream->ReadByte();
                uint8 green = stream->ReadByte();
                uint8 blue = stream->ReadByte();
                uint8 alpha = stream->ReadByte();

                image.SetPixel(w, h, red, green, blue, alpha);
            }
        }

        images.push_back(image);
        width /= 2;
        height /= 2;

    } while (images.size() < number_mipmaps);

    return images;
}
