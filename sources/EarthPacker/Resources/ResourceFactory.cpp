// 2022/09/23 20:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/ResourceFactory.h"
#include "Resources/Interface.h"
#include "Resources/Level.h"
#include "Resources/Terrain.h"
#include "Resources/Group.h"
#include "Resources/Mesh.h"


namespace ResourceFactory
{
    static std::string GetName(wxInputStream &);

    static int GetLength(wxInputStream &);

    static ResourceInfo GetResourceInfo(wxInputStream &);

    static std::vector<uint8> GetBytes(wxInputStream &, int);
}


Resource ResourceFactory::_Create(wxInputStream &stream)
{
    auto name = GetName(stream);

    TypeResource type = TypeResource::_0;

    stream.Read(&type, 1);

    switch (type)
    {
    case TypeResource::_0:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::Generic:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::Player:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::Text:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::Interface:
        {
            ResourceInfo info = GetResourceInfo(stream);
            auto name_resouce = GetName(stream);
            return Interface(name, info, name_resouce);
        }

    case TypeResource::Dialog:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::Interface2:
        return Resource(name, GetResourceInfo(stream));

    case TypeResource::_33:
        {
            ResourceInfo info = GetResourceInfo(stream);
            auto data = GetBytes(stream, 16);
            return Mesh(name, info, &data);
        }

    case TypeResource::Level:
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


std::string ResourceFactory::GetName(wxInputStream &stream)
{
    int length = GetLength(stream);

    wxMemoryBuffer nameByte(length + 1);

    stream.Read(nameByte.GetData(), length);

    ((uint8 *)nameByte.GetData())[length] = 0;

    return std::string((char *)nameByte.GetData());
}


int ResourceFactory::GetLength(wxInputStream &stream)
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


ResourceInfo ResourceFactory::GetResourceInfo(wxInputStream &stream)
{
    ResourceInfo result{0, 0, 0};

    stream.Read(&result.offset, 4);

    stream.Read(&result.length, 4);

    stream.Read(&result.decompressedLength, 4);

    return result;
}


std::vector<uint8> ResourceFactory::GetBytes(wxInputStream &stream, int length)
{
    std::vector<uint8> result(length);

    stream.Read(result.data(), length);

    return result;
}
