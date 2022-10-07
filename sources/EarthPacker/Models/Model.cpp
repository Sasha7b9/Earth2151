// 2022/09/24 14:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Model.h"
#include "Utils/FileInputStream.h"
#include "Models/Collections/MountPoints.h"
#include "Resources/Mesh.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


Model::Model(const wxFileName &_file_name)
{
    file_name = _file_name;

    FileInputStream stream(file_name.GetFullPath());

    CheckHeader(stream);

    type = ReadType(stream);

    model_template.Create(stream, description);

    ReadBytes(stream, "Unused", 10);

    mount_points = new MountPoints(stream, description);

    for (int i = 0; i < Light::COUNT; i++)
    {
        lights.push_back(Light(stream, i));
    }

    unused2_64 = stream.ReadBytes(64);
    unused3_488 = stream.ReadBytes(488);

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


void Model::ReadBytes(FileInputStream &stream, pchar name, int num_bytes)
{
    InfoModel info(stream.TellI(), num_bytes, name);

    info.AppendBytes(stream.ReadBytes(num_bytes));

    description.AppendInfo(info);
}


void Model::GetDescription(DescriptionModel *_desc)
{
    *_desc = description;
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


void Model::CheckHeader(FileInputStream &stream)
{
    InfoModel info(stream.TellI(), 8, "Header");

    wxMemoryBuffer span = stream.ReadBytes(8);

    uint8 template_buffer[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(span.GetData(), template_buffer, 8) != 0)
    {
        LOG_ERROR("Not mesh format");
    }

    info.AppendBytes(span);

    description.AppendInfo(info);
}


int Model::ReadType(FileInputStream &stream)
{
    InfoModel info(stream.TellI(), 4, "Type");

    int result = stream.ReadUINT();

    info.AppendBytes(&result, 4);

    description.AppendInfo(info);

    return result;
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


void DescriptionModel::AppendInfo(const InfoModel &info)
{
    push_back(info);
}


int DescriptionModel::Size() const
{
    return size();
}


void DescriptionModel::DrawLine(const PageInfo *page, int y, int num_lines) const
{
    const InfoModel &info = (*this)[num_lines];

    int width = page->GetClientSize().GetWidth();

    page->DrawLine(0, y, 0, y + PageInfo::PIXELS_IN_LINE);
    page->DrawLine(0, y + PageInfo::PIXELS_IN_LINE, width, y + PageInfo::PIXELS_IN_LINE);
    page->DrawLine(width - 1, y, width - 1, y + PageInfo::PIXELS_IN_LINE);

    int x = DrawCell(page, 0, y, 90, info.name);

    x = DrawCell(page, x, y, 50, info.address);

    x = DrawCell(page, x, y, 50, info.size);

    x = DrawCell(page, x, y, 725, info.bytes, 48);
}


InfoModel::InfoModel(FileInputStream &stream, int num_bytes, pchar _name)
{
    address = (uint)stream.TellI();

    name = _name;

    wxMemoryBuffer data = stream.ReadBytes(num_bytes);

    for (uint i = 0; i < data.GetBufSize(); i++)
    {
        bytes.push_back(data[i]);
    }
}


void InfoModel::Append(Vector &vector)
{
    AppendBytes(&vector.x, sizeof(vector.x));
    AppendBytes(&vector.y, sizeof(vector.y));
    AppendBytes(&vector.z, sizeof(vector.z));
}
