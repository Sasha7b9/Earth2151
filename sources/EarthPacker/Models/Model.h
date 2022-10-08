// 2022/09/24 14:55:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/ModelPart.h"
#include "Utils/FileInputStream.h"
#include "Models/Elements/ModelTemplate.h"
#include "Models/Collections/MountPoints.h"
#include "Models/PartNode.h"
#include "Resources/Description.h"
#include "Models/Elements/Light.h"


struct HeaderInfoModel
{
    int offset;
    std::string name;
};


struct InfoModel
{
    InfoModel(int offset, pchar name);
    InfoModel(uint offset, pchar name);
    InfoModel(wxFileOffset offset, pchar name, int size = 0);

    InfoModel &AppendBytes(const wxMemoryBuffer &);
    InfoModel &AppendBytes(void *data, int num_bytes);
    InfoModel &AppendBytes(Vector &);
    InfoModel &AppendBytes(float);

    HeaderInfoModel header;
    int size;
    std::vector<uint8> bytes;
};


struct DescriptionModel : public Description, public std::map<HeaderInfoModel, InfoModel>
{
    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_line) const override;

    void AppendInfo(const InfoModel &);
    void AppendInfo(InfoModel &, FileInputStream &);

private:

    InfoModel &GetInfo(int num_line);
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
    ModelTemplate model_template;
    MountPoints *mount_points = nullptr;
    std::vector<Light> lights;

    PartNode *partsTree = nullptr;

    DescriptionModel description;

    void CheckHeader(FileInputStream &);

    int ReadType(FileInputStream &);

    void ReadBytes(FileInputStream &, pchar name, int num_bytes); 

    void GetParts(FileInputStream &, std::list<ModelPart *> &);

    PartNode *GetPartsTree();
};
