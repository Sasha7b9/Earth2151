// 2022/09/24 14:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Model.h"
#include "Utils/FileInputStream.h"
#include "Models/Collections/MountPoints.h"
#include "Resources/Mesh.h"


Model::Model(const wxFileName &file_name)
{
    FileInputStream stream(file_name.GetFullPath());

    CheckHeader(stream);

    type = stream.ReadUINT();

    model_template = new ModelTemplate(stream);

    stream.ReadBytes(10);

    mount_points = new MountPoints(stream);

    lights = new Lights(stream);

    stream.ReadBytes(64);
    stream.ReadBytes(488);

    unknown_val1 = stream.Read2Bytes();
    unknown_val2 = stream.Read2Bytes();
    unknown_val3 = stream.Read2Bytes();
    unknown_val4 = stream.Read2Bytes();
    unknown_val5 = stream.ReadUINT();

    if (type != 0)
    {
        LOG_ERROR("Not supported mesh format");
    }

    GetParts(stream, parts);

    partsTree = GetPartsTree();
}

PartNode *Model::GetPartsTree()
{
    int currentID = 0;
    PartNode * const root = new PartNode(currentID, *parts.begin());
    PartNode *lastNode = root;

    for each (auto part in parts)
    {
        if (part == *parts.begin())
        {
            continue;
        }

        int skip = part->skipParent;
        PartNode *parent = lastNode;

        for (int i = 0; i < skip; i++)
        {
            parent = parent->parent;
        }
        lastNode = new PartNode(++currentID, part, parent);
    }

    return root;
}


void Model::SaveToXML(const wxFileName &)
{
//    wxXmlDocument xml;
//
//    wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, "Root", "Content");
//    xml.SetRoot(root);
//
//    wxObjectXmlWriter 
}


void Model::CheckHeader(FileInputStream &stream)
{
    wxMemoryBuffer span = stream.ReadBytes(8);

    uint8 template_buffer[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(span.GetData(), template_buffer, 8) != 0)
    {
        LOG_ERROR("Not mesh format");
    }
}


void Model::GetParts(FileInputStream &stream, std::list<ModelPart *> &_parts)
{
    _parts.clear();

    while (stream.TellI() < stream.GetSize())
    {
//        size_t size = stream.GetSize();
//        wxFileOffset pos = stream.TellI();

        _parts.push_back(new ModelPart(stream));
    }
}
