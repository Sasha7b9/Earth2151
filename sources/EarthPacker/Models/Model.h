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
    int offset = 0;
    std::string name;

    bool operator<(const HeaderInfoModel &rhs) const
    {
        return (offset == rhs.offset) ? (name < rhs.name) : (offset < rhs.offset);
    }
};


struct InfoModel
{
    enum class Type
    {
        _None,
        MountPoints,
        NotSupportedMeshFormat,
        Vertices,
        Face,
        Light,
        ModelTemplate,
        UnusedBytes,
        Header,
        Type,
        ModelPart
    };

    InfoModel(Type type, uint offset, pchar name);
    InfoModel(Type type, wxFileOffset offset, pchar name);

    InfoModel &AppendBytes(const wxMemoryBuffer &);
    InfoModel &AppendBytes(const void *data, int num_bytes);
    InfoModel &AppendBytes(const Vector &);
    InfoModel &AppendBytes(const float);

    HeaderInfoModel header;
    int size = 0;
    std::vector<uint8> bytes;
    Type type = Type::_None;

    // Здесь хранятся строки с описанием поля
    struct Content
    {
        Content(InfoModel &_info) : info(_info) {}
        pchar First();
        pchar Next();
    private:
        std::vector<std::string> content;
        static const int length_name = 20;      // Количество знакомест на описание раздела
        int current_line = 0;                   // Эту строку нужно выводить сейчас
        InfoModel &info;
        bool IsCreated() const { return content.size() != 0; }
        void Create();
        void CreateBeginLine(std::string &);
    } content;
};


struct DescriptionModel : public Description, public std::map<HeaderInfoModel, InfoModel>
{
    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_line) const override;

    void AppendInfo(const InfoModel &);
    void AppendInfo(InfoModel &, FileInputStream &);

    void Log();

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

    void ReadUnusedBytes(FileInputStream &, pchar name, int num_bytes); 

    void GetParts(FileInputStream &, std::list<ModelPart *> &);

    PartNode *GetPartsTree();
};
