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
    uint address;
    int size;
    std::string type;
    std::vector<uint8> bytes;
    void Append4Bytes(uint value)
    {
        for (int i = 0; i < 4; i++)
        {
            bytes.push_back(value & 0xFF);
            value >>= 8;
        }
    }
};


class DescriptionModel : public std::vector<InfoModel>, public Description
{
public:
    void AppendLine(const InfoModel &);
    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_lines) const override;
private:
};


class Model
{
public:

    Model(const wxFileName &file_name);

    void GetDescription(DescriptionModel *out);

private:

    wxFileName file_name;
    std::list<ModelPart *> parts;
    int type;
    ModelTemplate *model_template = nullptr;
    MountPoints *mount_points = nullptr;
    std::vector<Light> lights;

    uint16 unknown_val1 = 0;
    uint16 unknown_val2 = 0;
    uint16 unknown_val3 = 0;
    uint16 unknown_val4 = 0;
    uint   unknown_val5 = 0;

    PartNode *partsTree = nullptr;

    void CheckHeader(FileInputStream &);

    void GetParts(FileInputStream &, std::list<ModelPart *> &);

    PartNode *GetPartsTree();

    InfoModel GetHeader(FileInputStream &);
};
