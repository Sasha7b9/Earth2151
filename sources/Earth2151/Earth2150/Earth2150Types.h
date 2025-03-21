// 2025/03/06 14:15:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"
#include "Utils/StringUtils.h"
#include "Graphics/CanvasTexture.h"


struct LObject
{
    LObject() : jobCreateObject{ &JobCreateObject, this }
    {

    }

    LObject(const LObject &obj) :
        jobCreateObject{ &JobCreateObject, this }
    {
        description = obj.description;
        _position = obj._position;
        underground = obj.underground;
        rotation_rad = obj.rotation_rad;

        for (int i = 0; i < obj.values.GetArrayElementCount(); i++)
        {
            values.AppendArrayElement(obj.values[i]);
        }

        for (int i = 0; i < obj.strings.GetArrayElementCount(); i++)
        {
            strings.AppendArrayElement(obj.strings[i]);
        }
    }

    String<> description;

    Point3D _position;
    bool    underground;
    float   rotation_rad;           // Измеряется в радианах

    Array<int>      values;
    Array<String<>> strings;

    String<> CreateFullDescription(int num);

    bool IsEmpty() const
    {
        return description.GetStringLength() == 0;
    }

    class CreateObjectJob : public BatchJob
    {
    public:
        CreateObjectJob(ExecuteCallback *callback = nullptr, LObject *object = nullptr) :
            BatchJob(callback, object)
        {
        }
    };

    CreateObjectJob jobCreateObject;

    static void JobCreateObject(Job *, void *);
};

struct LObjects
{
    void ReadObjects(HeapBuffer &);

    void SaveToFileTXT(pchar);

    // Здесь хранится информация об объектах из файла .mis
    Array<LObject> objects;

private:

    // siz_size - размер ячейки хранения размера
    String<> ReadString(int size_size, HeapBuffer &);

    void ReadMarkers(HeapBuffer &);

    void ReadUnit(HeapBuffer &, LObject &);
};


// Содержит информацию о ландшафте из файлов .lnd, .mis в raw-виде
struct Level2150
{
    Level2150(const String<> &file_name) :
        file_name_lnd{file_name + ".lnd"}, file_name_mis{file_name + ".mis"}
    {
    }

    String<> file_name_lnd;
    String<> file_name_mis;

    HeapBuffer dataLND{ 10 * 1024 };
    HeapBuffer mis{ 10 * 1024 };

    // Читает объекты, из .mis-файла
    void ReadObjects();

    LObjects objects;

    void Destroy()
    {
        dataLND.Destroy();
        mis.Destroy();
    }
};


struct Image2150
{
    Image2150(int _width, int _height) : width(_width), height(_height)
    {
        if (_width * _height == 65536)
        {
            _width = _width;
        }

        storage = new ColorRGBA[width * height];
    }

    ~Image2150()
    {
        if (storage)
        {
            delete []storage;
            storage = nullptr;
        }
    }

    int width;
    int height;

    ColorRGBA *storage = nullptr;
};


struct Texture2150
{
    struct Type
    {
        uint8 value;

        bool IsTexture() const
        {
            return value & 2;
        }

        bool IsLod() const
        {
            return value & 4;
        }

    } type;

    struct SubType
    {
        uint8 value;

        bool IsUnknownFlag1() const
        {
            return value & 1;
        }

        bool IsUnknownFlag2() const
        {
            return value & 2;
        }

        bool IsSides() const
        {
            return value & 16;
        }

        bool IsCollection() const
        {
            return value & 64;
        }

        bool IsGrouped() const
        {
            return value & 128;
        }

    } sub_type;

    uint8 reserved = 0;
    uint8 reserved2 = 0;
    uint8 reserved3 = 0;
    uint8 unknown1 = 0;
    uint8 unknown2 = 0;

    int group_count = 0;
    uint8 animation_type = 0;

    int width = 0;
    int height = 0;

    int lod_levels = 0;

    int count = 0;

    Image2150 *image = nullptr;       // Если это это набор текстур, то здесь nullptr

    CanvasTexture *canvas = nullptr;

    bool Read(FileReader &);

    void Destroy()
    {
        delete image;
        image = nullptr;

        delete canvas;
        canvas = nullptr;
    }
};


// Сюда читается информация из файла TEX
struct FileTex2150
{
    FileTex2150(const String<> &tex = "") : file_name_tex(tex)
    {
    }

    String<>            file_name_tex;      // В этом файеле хранятся текстуры
    Array<Texture2150>  textures;           // А это текстуры как они есть - от первой к последней. Нулевой идёт заглавная текстура

    CanvasTexture *canvas = nullptr;

    ~FileTex2150()
    {
        Destroy();
    }

    void Destroy()
    {
        for each(Texture2150 & tex in textures)
        {
            tex.Destroy();
        }

        textures.PurgeArray();

        if (canvas)
        {
            delete canvas;
            canvas = nullptr;
        }
    }
};
