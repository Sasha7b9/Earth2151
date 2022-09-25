// 2022/09/23 20:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Packer/Resources/ResourceFactory.h"
#include "Packer/Resources/Interface.h"
#include "Packer/Resources/Level.h"
#include "Packer/Resources/Terrain.h"
#include "Packer/Resources/Group.h"


namespace Packer
{
    namespace ResourceFactory
    {
        static std::string GetName(wxInputStream &);

        static int GetLength(wxInputStream &);

        static ResourceInfo GetResourceInfo(wxInputStream &);

        static std::vector<uint8> GetBytes(wxInputStream &, int);

        Resource ResourceFactory::Create(wxInputStream &stream)
        {
            auto name = GetName(stream);

            uint8 type = 0;

            stream.Read(&type, 1);

            switch (type)
            {
            case 0:
                return Resource(name, GetResourceInfo(stream));

            case 1:     // generic data
                return Resource(name, GetResourceInfo(stream));

            case 3:     //player
                return Resource(name, GetResourceInfo(stream));

            case 5:     //text
                return Resource(name, GetResourceInfo(stream));

            case 9:     //interface?
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto name_resouce = GetName(stream);
                return Interface(name, info, name_resouce);
            }

            case 17:    //dialog
                return Resource(name, GetResourceInfo(stream));

            case 25:    //interface?
                return Resource(name, GetResourceInfo(stream));

            case 33:
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto data = GetBytes(stream, 16);
                return Mesh(name, info, &data);
            }

            case 43:    //level?
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto name_resource = GetName(stream);
                auto data = GetBytes(stream, 16);
                return Level(name, info, name_resource, &data);
            }

            case 49:    // mesh
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto data = GetBytes(stream, 20);
                return Mesh(name, info, &data);
            }

            case 57:
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto name_resource = GetName(stream);
                auto data = GetBytes(stream, 20);
                return Terrain(name, info, name_resource, &data);
            }

            case 59:    //level
            {
                ResourceInfo info = GetResourceInfo(stream);
                auto name_resource = GetName(stream);
                auto data = GetBytes(stream, 20);
                return Level(name, info, name_resource, &data);
            }

            case 255:   //group
            {
                auto data = GetBytes(stream, 3);
                return Group(name, ResourceInfo(), &data);
            }
            }

            return Resource(name, GetResourceInfo(stream));
        }
    }
}


std::string Packer::ResourceFactory::GetName(wxInputStream &stream)
{
    int length = GetLength(stream);

    wxMemoryBuffer nameByte(length + 1);

    stream.Read(nameByte.GetData(), length);

    ((uint8 *)nameByte.GetData())[length] = 0;

    return std::string((char *)nameByte.GetData());
}


int Packer::ResourceFactory::GetLength(wxInputStream &stream)
{
    uint8 length = 0;
    stream.Read(&length, 1);

    if (length == 73)
    {
        wxFileOffset current = stream.TellI();

        wxMemoryBuffer idxBuffer(3);
        stream.Read(idxBuffer.GetData(), 3);

        if (!(idxBuffer[0] == 68 && idxBuffer[1] == 0))
        {
            stream.SeekI(current);
        }
        else
        {
            stream.Read(&length, 1);
        }
    }

    return length;
}


Packer::ResourceInfo Packer::ResourceFactory::GetResourceInfo(wxInputStream &stream)
{
    ResourceInfo result{0, 0, 0};

    stream.Read(&result.offset, 4);

    stream.Read(&result.length, 4);

    stream.Read(&result.decompressedLength, 4);

    return result;
}


std::vector<uint8> Packer::ResourceFactory::GetBytes(wxInputStream &stream, int length)
{
    std::vector<uint8> result(length);

    stream.Read(result.data(), length);

    return result;
}
