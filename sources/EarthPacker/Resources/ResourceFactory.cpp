// 2022/09/23 20:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/ResourceFactory.h"


using namespace Resources;


namespace Resources
{
    namespace ResourceFactory
    {
        static std::string GetName(wxInputStream &);

        static int GetLength(wxInputStream &);

        static ResourceInfo GetResourceInfo(wxInputStream &);
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
    }

    return Resource(name, GetResourceInfo(stream));
}


std::string ResourceFactory::GetName(wxInputStream &stream)
{
    int length = GetLength(stream);

    length = length;

    return "";
}


int ResourceFactory::GetLength(wxInputStream &stream)
{
    uint8 length = 0;
    stream.Read(&length, 1);

    if (length == 73)
    {
        wxFileOffset current = stream.TellI();

        wxMemoryBuffer idxBuffer(3);
        stream.SeekI(0);
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


ResourceInfo ResourceFactory::GetResourceInfo(wxInputStream &)
{
    ResourceInfo result{0, 0, 0};

    return result;
}