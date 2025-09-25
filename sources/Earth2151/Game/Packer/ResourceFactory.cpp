// 2022/09/23 20:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Packer/MemoryInputStream.h"
#include "Game/Packer/ResourceWD.h"
#include "Game/Packer/ResourceFactory.h"
#include "Game/Packer/Interface.h"
#include "Game/Packer/MeshWD.h"
#include "Game/Packer/LevelWD.h"
#include "Game/Packer/TerrainWD.h"
#include "Game/Packer/GroupWD.h"


namespace ResourceFactory
{
    static String<> GetName(MemoryInputStream &);

    static int GetLength(MemoryInputStream &);

    static ResourceInfoWD GetResourceInfo(MemoryInputStream &);

    static std::vector<uint8> GetBytes(MemoryInputStream &, int);
}


ResourceWD ResourceFactory::Create(MemoryInputStream &stream)
{
    auto name = GetName(stream);

    TypeResource type = TypeResource::_0;

    stream.Read(&type, 1);

    switch (type)
    {
    case TypeResource::_0:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::Generic:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::Player:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::Text:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::Interface:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto name_resouce = GetName(stream);
            return Interface(name, info, name_resouce);
        }

    case TypeResource::Dialog:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::Interface2:
        return ResourceWD(name, GetResourceInfo(stream));

    case TypeResource::_33:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto data = GetBytes(stream, 16);
            return MeshWD(name, info, &data);
        }

    case TypeResource::Level:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto name_resource = GetName(stream);
            auto data = GetBytes(stream, 16);
            return LevelWD(name, info, name_resource, &data);
        }

    case TypeResource::Mesh:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto data = GetBytes(stream, 20);
            return MeshWD(name, info, &data);
        }

    case TypeResource::_57:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto name_resource = GetName(stream);
            auto data = GetBytes(stream, 20);
            return TerrainWD(name, info, name_resource, &data);
        }

    case TypeResource::Level2:
        {
            ResourceInfoWD info = GetResourceInfo(stream);
            auto name_resource = GetName(stream);
            auto data = GetBytes(stream, 20);
            return LevelWD(name, info, name_resource, &data);
        }

    case TypeResource::Group:
        {
            auto data = GetBytes(stream, 3);
            return GroupWD(name, ResourceInfoWD(), &data);
        }
    }

    return ResourceWD(name, GetResourceInfo(stream));
}


String<> ResourceFactory::GetName(MemoryInputStream &stream)
{
    int length = GetLength(stream);

    MemoryBufferWD nameByte(length + 1);

    stream.Read(nameByte.GetData(), length);

    ((uint8 *)nameByte.GetData())[length] = 0;

    return (char *)nameByte.GetData();
}


int ResourceFactory::GetLength(MemoryInputStream &stream)
{
    uint8 length = 0;
    stream.Read(&length, 1);

    if (length == 73)
    {
        const int offset = stream.TellI();

        MemoryBufferWD idxBuffer(3);
        stream.Read(idxBuffer.GetData(), 3);

        if (!(idxBuffer[0] == 68 && idxBuffer[1] == 0))
        {
            stream.SeekI(offset);
        }
        else
        {
            stream.Read(&length, 1);
        }
    }

    return length;
}


ResourceInfoWD ResourceFactory::GetResourceInfo(MemoryInputStream &stream)
{
    ResourceInfoWD result{0, 0, 0};

    stream.Read(&result.offset, 4);

    stream.Read(&result.length, 4);

    stream.Read(&result.decompressedLength, 4);

    return result;
}


std::vector<uint8> ResourceFactory::GetBytes(MemoryInputStream &stream, int length)
{
    std::vector<uint8> result(length);

    stream.Read(result.data(), length);

    return result;
}
