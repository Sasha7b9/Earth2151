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
    string name;

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
        ModelPart,
        Vector,
        Color,
        Float,
        UINT,
        VerticesBlock,
        Mat44,
        RotationFrames
    };

    InfoModel(Type type, uint offset, pchar name = "");
    InfoModel(Type type, wxFileOffset offset, pchar name = "");

    InfoModel &AppendBytes(const wxMemoryBuffer &);
    InfoModel &AppendBytes(const void *data, int num_bytes);
    InfoModel &AppendBytes(const Vector &);
    InfoModel &AppendBytes(const float);
    InfoModel &AppendBytes(const uint);

    HeaderInfoModel header;
    int size = 0;
    vector<uint8> bytes;
    Type type = Type::_None;

    // «десь хран€тс€ строки с описанием пол€
    struct Content
    {
        pchar First(InfoModel &);
        pchar Next();
    private:
        vector<string> content;
        static const int length_title = 47;     //  оличество знакомест на заголовок (символы до содежимого блока)
        static const int bytes_in_line = 32;
        int current_line = 0;                   // Ёту строку нужно выводить сейчас
        int shown_bytes = 0;                    // „исло выведенных байт
        bool IsCreated() const { return content.size() != 0; }
        void Create(InfoModel &);
        // ¬озвращает количество выведенных байт из массива
        void CreateBeginLine(string &, InfoModel &);
        void CreateEngBeginLine(string &, InfoModel &);
        void CreateNextLine(string &, InfoModel &);
        void CreateEndNextLine(string &, InfoModel &);
        void PrepareForEndBeginLine(string &);
    } content;
};


struct DescriptionModel : public Description, public map<HeaderInfoModel, InfoModel>
{
    static void Set(DescriptionModel *desc) { current = desc; };
    static DescriptionModel *Get() { return current; };

    virtual int Size() const override;
    virtual void DrawLine(const PageInfo *, int y, int num_line) const override;

    void AppendInfo(const InfoModel &);
    void AppendInfo(InfoModel &, FileInputStream &);

    void Log();

private:

    static DescriptionModel *current;

    InfoModel &GetInfo(int num_line);
};


class Model
{
public:

    Model(const wxFileName &file_name);

    void GetDescription(DescriptionModel *out);

private:

    wxFileName file_name;
    list<ModelPart *> parts;
    int type;
    ModelTemplate model_template;
    MountPoints *mount_points = nullptr;
    vector<Light> lights;

    PartNode *partsTree = nullptr;

    DescriptionModel description;

    void CheckHeader(FileInputStream &);

    int ReadType(FileInputStream &);

    void ReadUnusedBytes(FileInputStream &, pchar name, int num_bytes); 

    void GetParts(FileInputStream &, list<ModelPart *> &);

    PartNode *GetPartsTree();
};
