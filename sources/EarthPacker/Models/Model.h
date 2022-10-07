// 2022/09/24 14:55:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/ModelPart.h"
#include "Utils/FileInputStream.h"
#include "Models/Elements/ModelTemplate.h"
#include "Models/Collections/MountPoints.h"
#include "Models/PartNode.h"
#include "Resources/Description.h"
#include "Models/Elements/Light.h"


struct InfoModel
{
    InfoModel(int _address, int _size, const std::string _name) : address(_address), size(_size), name(_name) {}
    InfoModel(wxFileOffset _address, int _size, const std::string _name) : address((int)_address), size(_size), name(_name) {}
    InfoModel() {};
    InfoModel(FileInputStream &, int num_bytes, pchar name);

    int address;
    int size;
    std::string name;
    std::vector<uint8> bytes;

    void AppendBytes(const wxMemoryBuffer &buffer)
    {
        for (uint i = 0; i < buffer.GetBufSize(); i++)
        {
            bytes.push_back(buffer[i]);
        }
    }

    void AppendBytes(void *value, int num_bytes)
    {
        uint8 *pointer = (uint8 *)value;

        for (int i = 0; i < num_bytes; i++)
        {
            bytes.push_back(*pointer++);
        }
    }
};


struct DescriptionModel : public std::vector<InfoModel>, public Description
{
public:
    void AppendInfo(const InfoModel &);
    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_lines) const override;
private:
};


class Model
{
public:

    Model(const wxFileName &file_name);

    void GetDescription(DescriptionModel *);

private:

    wxFileName file_name;
    std::list<ModelPart *> parts;
    int type;
    ModelTemplate model_template;
    MountPoints *mount_points = nullptr;
    std::vector<Light> lights;

    uint16 unknown_val1 = 0;
    uint16 unknown_val2 = 0;
    uint16 unknown_val3 = 0;
    uint16 unknown_val4 = 0;
    uint   unknown_val5 = 0;

    wxMemoryBuffer unused2_64;
    wxMemoryBuffer unused3_488;

    PartNode *partsTree = nullptr;

    DescriptionModel description;

    void CheckHeader(FileInputStream &);

    int ReadType(FileInputStream &);

    void ReadBytes(FileInputStream &, pchar name, int num_bytes);

    void GetParts(FileInputStream &, std::list<ModelPart *> &);

    PartNode *GetPartsTree();
};
