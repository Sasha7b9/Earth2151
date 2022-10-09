// 2022/09/24 14:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Model.h"
#include "Utils/FileInputStream.h"
#include "Models/Collections/MountPoints.h"
#include "Resources/Mesh.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


DescriptionModel *DescriptionModel::current = nullptr;


Model::Model(const wxFileName &_file_name) : file_name(_file_name)
{
    LOG_WRITE("%s", file_name.GetFullPath().c_str().AsChar());

    FileInputStream stream(file_name.GetFullPath());

    DescriptionModel::Set(&description);
    FileInputStream::Set(&stream);

    CheckHeader(stream);

    type = ReadType(stream);

    model_template.Create(stream, description);

    ReadUnusedBytes(stream, "Unused", 10);

    mount_points = new MountPoints(stream, description);

    for (int i = 0; i < Light::COUNT; i++)
    {
        lights.emplace_back(Light(stream, description, wxString::Format("Light %d", i)));
    }

    ReadUnusedBytes(stream, "Unused", 64);
    ReadUnusedBytes(stream, "Unused", 488);
    ReadUnusedBytes(stream, "Unused", 2);

    ReadUnusedBytes(stream, "Unused", 2);
    ReadUnusedBytes(stream, "Unused", 2);
    ReadUnusedBytes(stream, "Unused", 2);
    ReadUnusedBytes(stream, "Unused", 4);

    if (type != 0)
    {
        LOG_ERROR("Not supported mesh format");

        description.AppendInfo(InfoModel(InfoModel::Type::NotSupportedMeshFormat, 0xFFFFFFFF, "Not supported mesh format"));
    }
    else
    {
        GetParts(stream, parts);

        partsTree = GetPartsTree();
    }
}


void Model::ReadUnusedBytes(FileInputStream &stream, pchar name, int num_bytes)
{
    InfoModel info(InfoModel::Type::UnusedBytes, stream.TellI(), name);

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
    InfoModel info(InfoModel::Type::Header, stream.TellI(), "Header");

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
    InfoModel info(InfoModel::Type::Type, stream.TellI(), "Type resource");

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


InfoModel::InfoModel(Type _type, wxFileOffset _offset, pchar _name) : header{(int)_offset, _name}, type(_type)
{

}


InfoModel::InfoModel(Type _type, uint _offset, pchar _name) : header{ (int)_offset, _name }, type(_type)
{

}


InfoModel &InfoModel::AppendBytes(const void *data, int num_bytes)
{
    uint8 *pointer = (uint8 *)data;

    bytes.insert(bytes.begin(), pointer, pointer + num_bytes);

    size += num_bytes;

    return *this;
}


InfoModel &InfoModel::AppendBytes(const Vector &vector)
{
    AppendBytes(&vector.x, sizeof(vector.x));
    AppendBytes(&vector.y, sizeof(vector.y));
    AppendBytes(&vector.z, sizeof(vector.z));

    return *this;
}


InfoModel &InfoModel::AppendBytes(const float value)
{
    AppendBytes(&value, sizeof(value));

    return *this;
}


InfoModel &InfoModel::AppendBytes(const wxMemoryBuffer &buffer)
{
    AppendBytes(buffer.GetData(), buffer.GetBufSize());

    return *this;
}


void DescriptionModel::AppendInfo(InfoModel &info, FileInputStream &stream)
{
    info.size = (int)stream.TellI() - info.header.offset;

    AppendInfo(info);
}


void DescriptionModel::Log()
{
    for (auto &info : *this)
    {
        pchar line = info.second.content.First(info.second);

        while (line)
        {
            LOG_WRITE(line);
            line = info.second.content.Next();
        }
    }

    LOG_WRITE("Unparsed bytes:");

    int last_address = 0;

    for (auto &info : *this)
    {
        if (info.first.offset > last_address)
        {
            last_address = info.first.offset;
        }
    }

    string line;

    for (int address = 0; address < last_address; address++)
    {
        bool exist = false;

        for (auto &info : *this)
        {
            if (address >= info.first.offset &&
                address < (info.first.offset + info.second.size))
            {
                exist = true;
                break;
            }
        }

        if (!exist)
        {
            line.append(wxString::Format("%d ", address).c_str());

            if (line.size() > 200)
            {
                break;
            }
        }
    }

    LOG_WRITE(line.c_str());
}


pchar InfoModel::Content::First(InfoModel &info)
{
    if (!IsCreated())
    {
        Create(info);
    }

    current_line = 0;
    shown_bytes = 0;

    return Next();
}


pchar InfoModel::Content::Next()
{
    if (current_line < (int)content.size())
    {
        return content[current_line++].c_str();
    }

    return nullptr;
}


void InfoModel::Content::Create(InfoModel &info)
{
    std::string line;

    CreateBeginLine(line, info);

    content.push_back(line);

    while (shown_bytes < (int)info.bytes.size())
    {
        std::string next;

        CreateNextLine(next, info);

        content.push_back(next);
    }

}


void InfoModel::Content::CreateBeginLine(std::string &line, InfoModel &info)
{
    line.append(wxString::Format(" % 4X: % 5d | % 4X: % 5d | %s", info.header.offset, info.header.offset, info.size, info.size, info.header.name.c_str()).c_str().AsChar());

    while (line.size() < length_title)
    {
        line.append(" ");
    }

    line.append("|");

    for (uint i = 0; i < info.bytes.size(); i++)
    {
        line.append(wxString::Format(" % 2X", info.bytes[i]));
        shown_bytes++;

        if (shown_bytes == bytes_in_line)
        {
            break;
        }
    }

    CreateEngBeginLine(line, info);
}


void InfoModel::Content::CreateEngBeginLine(string &line, InfoModel &info)
{
    PrepareForEndBeginLine(line);

    if (info.type == Type::Header)
    {
        for (uint i = 0; i < info.bytes.size(); i++)
        {
            line.append(wxString::Format("%c", (char)info.bytes[i]));
        }
    }
    else if (info.type == Type::Type)
    {
        uint t = 0;
        memcpy(&t, info.bytes.data(), 4);

        line.append(wxString::Format("%d", t));
    }
    else if (info.type == Type::ModelTemplate)
    {
        uint16 bits = 0;
        memcpy(&bits, info.bytes.data(), 2);

        auto f = [](string &l, uint16 &v)
        {
            for (int i = 0; i < 4; i++)
            {
                l.append(wxString::Format("%c", (char)((1 & (v)) + 0x30)));
                v >>= 1;
            }
            l.append(" ");
        };

        for (int i = 0; i < 4; i++)
        {
            f(line, bits);
        }
    }
    else if (info.type == Type::Vector)
    {
        line.append(Vector(info.bytes.data()).ToString().c_str());
    }
    else if (info.type == Type::Color)
    {
        line.append(Vector(info.bytes.data()).ToString().c_str());
    }
    else if (info.type == Type::Float)
    {
        float value = 0.0f;
        memcpy(&value, info.bytes.data(), sizeof(value));
        line.append(wxString::Format("%f", value));
    }
}


void InfoModel::Content::PrepareForEndBeginLine(string &line)
{
    while (line.size() < 142)
    {
        line.append(" ");
    }

    line.append(": ");
}


void InfoModel::Content::CreateNextLine(string &line, InfoModel &info)
{
    line.append(wxString::Format("             |             |", info.header.offset, info.header.offset, info.size, info.size).c_str().AsChar());

    while (line.size() < length_title)
    {
        line.append(" ");
    }

    line.append("|");

    int num_bytes = 0;  // Выведено байт в данной строке

    for (uint i = shown_bytes; i < info.bytes.size(); i++)
    {
        line.append(wxString::Format(" % 2X", info.bytes[i]));
        shown_bytes++;
        num_bytes++;

        if (num_bytes == bytes_in_line)
        {
            break;
        }
    }
}
