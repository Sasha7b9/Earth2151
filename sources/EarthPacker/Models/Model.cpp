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

    LOG_WRITE("%s", file_name.GetFullPath().c_str().AsChar());

    FileInputStream stream(file_name.GetFullPath());

    CheckHeader(stream);

    type = ReadType(stream);

    model_template.Create(stream, description);

    ReadBytes(stream, "Unused", 10);

    mount_points = new MountPoints(stream, description);

    for (int i = 0; i < Light::COUNT; i++)
    {
        lights.push_back(Light(stream, description, wxString::Format("Light %d", i)));
    }

    ReadBytes(stream, "Unused", 64);
    ReadBytes(stream, "Unused", 488);
    ReadBytes(stream, "Unused", 2);

    ReadBytes(stream, "Unused", 2);
    ReadBytes(stream, "Unused", 2);
    ReadBytes(stream, "Unused", 2);
    ReadBytes(stream, "Unused", 4);

    if (type != 0)
    {
        LOG_ERROR("Not supported mesh format");

        description.AppendInfo(InfoModel(0xFFFFFFFF, "Not supported mesh format"));
    }
    else
    {
        GetParts(stream, parts);

        partsTree = GetPartsTree();
    }
}


void Model::ReadBytes(FileInputStream &stream, pchar name, int num_bytes)
{
    InfoModel info(stream.TellI(), name, num_bytes);

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

    for (auto part : parts)
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
    InfoModel info(stream.TellI(), "Header", 0);

    wxMemoryBuffer span = stream.ReadBytes(8);

    uint8 template_buffer[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(span.GetData(), template_buffer, 8) != 0)
    {
        LOG_ERROR("Not mesh format");
    }

    description.AppendInfo(info.AppendBytes(span));
}


int Model::ReadType(FileInputStream &stream)
{
    InfoModel info(stream.TellI(), "Type resource", 4);

    int result = stream.ReadUINT();

    description.AppendInfo(info.AppendBytes(&result, 4));

    return result;
}


void Model::GetParts(FileInputStream &stream, std::list<ModelPart *> &_parts)
{
    _parts.clear();

    int num_model = 0;

    while (stream.TellI() < stream.GetSize())
    {
        _parts.push_back(new ModelPart(stream, description, ++num_model));
        LOG_WRITE("Model part %d", num_model);
    }
}


void DescriptionModel::AppendInfo(const InfoModel &info)
{
    insert(std::pair{ info.header, info });
}


int DescriptionModel::Size() const
{
    return size();
}


void DescriptionModel::DrawLine(const PageInfo *, int, int) const
{
//    const InfoModel &info = (*this)[num_lines];
//
//    int width = page->GetClientSize().GetWidth();
//
//    page->DrawLine(0, y, 0, y + PageInfo::PIXELS_IN_LINE);
//    page->DrawLine(0, y + PageInfo::PIXELS_IN_LINE, width, y + PageInfo::PIXELS_IN_LINE);
//    page->DrawLine(width - 1, y, width - 1, y + PageInfo::PIXELS_IN_LINE);
//
//    int x = DrawCell(page, 0, y, 90, info.name);
//
//    x = DrawCell(page, x, y, 50, info.address);
//
//    x = DrawCell(page, x, y, 50, info.size);
//
//    x = DrawCell(page, x, y, 725, info.bytes, 48);
}


InfoModel::InfoModel(wxFileOffset _offset, pchar _name, int _size) : header{(int)_offset, _name}, size(_size)
{
    
}


InfoModel &InfoModel::AppendBytes(const void *data, int num_bytes)
{
    uint8 *pointer = (uint8 *)data;

    bytes.insert(bytes.begin(), pointer, pointer + num_bytes);

    return *this;
}


InfoModel &InfoModel::AppendBytes(const Vector &vector)
{
    AppendBytes((const void *)&vector.x, sizeof(vector.x));
    AppendBytes((const void *)&vector.y, sizeof(vector.y));
    AppendBytes((const void *)&vector.z, sizeof(vector.z));

    return *this;
}


void DescriptionModel::AppendInfo(InfoModel &info, FileInputStream &stream)
{
    info.size = (int)stream.TellI() - info.header.offset;

    AppendInfo(info);
}


bool HeaderInfoModel::operator<(const HeaderInfoModel &rhs) const
{
    return (offset < rhs.offset ? true : (name < rhs.name));
}
