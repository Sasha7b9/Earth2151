// 2022/09/24 14:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Model.h"
#include "Utils/FileInputStream.h"
#include "Models/Collections/MountPoints.h"
#include "Resources/Mesh.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


Model::Model(const wxFileName &_file_name) : file_name(_file_name)
{
    LOG_WRITE("%s", file_name.GetFullPath().c_str().AsChar());

    FileInputStream main_stream(file_name.GetFullPath());

    IInputStream::SetInputStream(&main_stream);
    IInputStream::SetDescriptionModel(&description);

    CheckHeader();

    type = ReadType();

    model_template.Create();

    ReadBytes("Unused", 10);

    mount_points = new MountPoints();

    for (int i = 0; i < Light::COUNT; i++)
    {
        lights.emplace_back(Light(wxString::Format(" Light %d", i)));
    }

    ReadBytes("Unused", 64);
    ReadBytes("Unused", 488);
    ReadBytes("Unused", 2);

    ReadBytes("Unused", 2);
    ReadBytes("Unused", 2);
    ReadBytes("Unused", 2);
    ReadBytes("Unused", 4);

    if (type != 0)
    {
        LOG_ERROR("Not supported mesh format");

        description.AppendInfo(InfoModel(InfoModel::Type::NotSupportedMeshFormat, 0xFFFFFFFF, "Not supported mesh format"));
    }
    else
    {
        GetParts(parts);

        partsTree = GetPartsTree();
    }
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


void Model::CheckHeader()
{
    InfoModel info(InfoModel::Type::Header, stream->TellI(), "Header");

    wxMemoryBuffer span = stream->ReadBytes(8);

    uint8 template_buffer[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(span.GetData(), template_buffer, 8) != 0)
    {
        LOG_ERROR("Not mesh format");
    }

    description.AppendInfo(info.AppendBytes(span));
}


int Model::ReadType()
{
    InfoModel info(InfoModel::Type::Type, stream->TellI(), "Type resource");

    int result = stream->ReadUINT();

    description.AppendInfo(info.AppendBytes(&result, 4));

    return result;
}


void Model::GetParts(std::list<ModelPart *> &_parts)
{
    _parts.clear();

    int num_model = 0;

    while (stream->TellI() < stream->GetSize())
    {
        _parts.push_back(new ModelPart(++num_model));
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


InfoModel &InfoModel::AppendBytes(const uint value)
{
    AppendBytes(&value, sizeof(value));

    return *this;
}


InfoModel &InfoModel::AppendByte(const uint8 value)
{
    AppendBytes(&value, sizeof(value));

    return *this;
}


InfoModel &InfoModel::AppendBytes(const wxMemoryBuffer &buffer)
{
    AppendBytes(buffer.GetData(), buffer.GetBufSize());

    return *this;
}


void DescriptionModel::AppendInfo(InfoModel &info, FileInputStream *stream)
{
    info.size = (int)stream->TellI() - info.header.offset;

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

//    LOG_WRITE("Unparsed bytes:");
//
//    int last_address = 0;
//
//    for (auto &info : *this)
//    {
//        if (info.first.offset > last_address)
//        {
//            last_address = info.first.offset;
//        }
//    }
//
//    string line;
//
//    for (int address = 0; address < last_address; address++)
//    {
//        bool exist = false;
//
//        for (auto &info : *this)
//        {
//            if (address >= info.first.offset &&
//                address < (info.first.offset + info.second.size))
//            {
//                exist = true;
//                break;
//            }
//        }
//
//        if (!exist)
//        {
//            line.append(wxString::Format("%d ", address).c_str());
//
//            if (line.size() > 200)
//            {
//                break;
//            }
//        }
//    }
//
//    LOG_WRITE(line.c_str());
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
    line.append(wxString::Format(" % 5X: % 7d | % 4X:% 6d | %s", info.header.offset, info.header.offset, info.size, info.size, info.header.name.c_str()).c_str().AsChar());

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
    else if (info.type == Type::Mat44)
    {
        for (int i = 0; i < 8; i++)
        {
            while (line.size() % 8)
            {
                line.append(" ");
            }
            float value = 0.0f;
            memcpy(&value, info.bytes.data() + i * sizeof(value), sizeof(value));
            line.append(wxString::Format("% 5.3f ", value));
        }
    }
    else if (info.type == Type::VerticesBlock)
    {
        auto f = [](string &l, uint8 *data, pchar format, int sign)
        {
            for (int i = 0; i < 4; i++)
            {
                float value = 0.0f;
                memcpy(&value, data + sizeof(value) * i, sizeof(value));
                value *= (float)sign;
                l.append(wxString::Format(format, i, value));
                while (l.size() % 15)
                {
                    l.append(" ");
                }
            }
        };

        f(line, info.bytes.data(), " x%d % 5f  ", 1);
        f(line, info.bytes.data() + sizeof(float) * 4, " y%d % 5f  ", -1);
    }
    else if (info.type == Type::Face)
    {
        uint8 *data = info.bytes.data();

        while (data < &info.bytes[info.bytes.size() - 1])
        {
            uint16 value = 0;

            memcpy(&value, data, 2);

            line.append(wxString::Format("% 3d", value));

            data += 2;
        }
    }
    else if (info.type == Type::UINT)
    {
        uint value = 0;

        memcpy(&value, info.bytes.data(), 4);

        line.append(wxString::Format("%d", value));
    }
    else if (info.type == Type::UINT16)
    {
        uint16 value = 0;
        memcpy(&value, info.bytes.data(), 2);
        line.append(wxString::Format("%d", value));
    }
    else if (info.type == Type::Byte)
    {
        uint8 value = 0;
        memcpy(&value, info.bytes.data(), 1);
        line.append(wxString::Format("%d", value));
    }
    else if (info.type == Type::Text)
    {
        string text;
        for (uint i = 0; i < info.bytes.size(); i++)
        {
            text.push_back((char)info.bytes[i]);
        }
        text.push_back('\0');
        line.append(text.c_str());
    }
}


void InfoModel::Content::CreateEndNextLine(string &line, InfoModel &info)
{
    PrepareForEndBeginLine(line);

    if (info.type == Type::VerticesBlock)
    {
        auto f = [](string &l, uint8 *data, pchar format, int sign)
        {
            for (int i = 0; i < 4; i++)
            {
                float value = 0.0f;
                memcpy(&value, data + sizeof(value) * i, sizeof(value));
                value *= (float)sign;
                l.append(wxString::Format(format, i, value));
                while (l.size() % 15)
                {
                    l.append(" ");
                }
            }
        };

        if (shown_bytes == 64)
        {
            f(line, info.bytes.data() + shown_bytes - bytes_in_line, " z%d % 5f  ", 1);
            f(line, info.bytes.data() + shown_bytes - bytes_in_line / 2, "nx%d % 5f  ", 1);
        }
        else if (shown_bytes == 96)
        {
            f(line, info.bytes.data() + shown_bytes - bytes_in_line, "ny%d % 5f  ", -1);
            f(line, info.bytes.data() + shown_bytes - bytes_in_line / 2, "nz%d % 5f  ", 1);
        }
        else if (shown_bytes == 128)
        {
            f(line, info.bytes.data() + shown_bytes - bytes_in_line, " u%d % 5f  ", -1);
            f(line, info.bytes.data() + shown_bytes - bytes_in_line / 2, " v%d % 5f  ", 1);
        }
    }
    else if (info.type == Type::Mat44)
    {
        for (int i = 0; i < 8; i++)
        {
            while (line.size() % 8)
            {
                line.append(" ");
            }
            float value = 0.0f;
            memcpy(&value, info.bytes.data() + i * sizeof(value) + bytes_in_line, sizeof(value));
            line.append(wxString::Format("% 5.3f ", value));
        }
    }
}


void InfoModel::Content::PrepareForEndBeginLine(string &line)
{
    while (line.size() < 145)
    {
        line.append(" ");
    }

    line.append(": ");
}


void InfoModel::Content::CreateNextLine(string &line, InfoModel &info)
{
    line.append(wxString::Format("                |             |", info.header.offset, info.header.offset, info.size, info.size).c_str().AsChar());

    while (line.size() < length_title)
    {
        line.append(" ");
    }

    line.append("|");

    int num_bytes = 0;  // �������� ���� � ������ ������

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

    CreateEndNextLine(line, info);
}
