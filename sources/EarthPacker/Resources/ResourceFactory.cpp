// 2022/09/23 20:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/ResourceFactory.h"
#include "Resources/Mesh.h"


using namespace Resources;


namespace Resources
{
    namespace ResourceFactory
    {
        static std::string GetName(wxInputStream &);

        static int GetLength(wxInputStream &);

        static ResourceInfo GetResourceInfo(wxInputStream &);

        static std::vector<uint8> GetBytes(wxInputStream &, int);
    }
}


Resource ResourceFactory::Create(wxInputStream &stream)
{
    auto name = GetName(stream);

    uint8 type = 0;

    stream.Read(&type, 1);

    switch (type)
    {
    case 0:
        return Resource(name, GetResourceInfo(stream));
    case 49:    // mesh
        ResourceInfo info = GetResourceInfo(stream);
        std::vector<uint8> data = GetBytes(stream, 20);
        return Mesh(name, info, &data);
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
