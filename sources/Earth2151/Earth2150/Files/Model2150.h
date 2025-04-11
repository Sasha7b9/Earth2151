// 2025/02/27 20:18:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Clock.h"


struct GeoData2150;


enum : Type
{
    kModel2150 = 'K215'
};


class ModelController : public Controller
{
public:
    ModelController() : Controller(kModel2150) { }
    void SetModel(GeoData2150 *geo)
    {
        model = geo;
    }
    virtual void MoveController() override;
    GeoData2150 *model = nullptr;
  
    int counter = 0;

    Node *main_node = nullptr;

    const uint speed_animation = 100;       // ms/frame Столько миллисекунд занимает один фрейм

    uint time_prev_frame = UCOUNT_MS;       // В это время применился предыдущий фрейм
};


struct Vertex
{
    Vector3D coord;
    Vector3D normal;
    Point2D  tex;
    int16    normID;
    int16    posID;

    void ConvertToGeometryVertex(GeometryVertex *);
};


struct GFace
{
    int16 index[4];     // \todo 4-ё значение непонятно как использовать
};


struct Animations
{
    Array<Vector3D>    scales;      // scale frames
    Array<Point3D>     trans;       // translation frames
    Array<Transform3D> rotations;   // rotations frames

    bool is_exist = true;

    int NumFrames()
    {
        int result = scales.GetArrayElementCount();

        if (trans.GetArrayElementCount() > result)
        {
            result = trans.GetArrayElementCount();
        }

        if (rotations.GetArrayElementCount() > result)
        {
            result = rotations.GetArrayElementCount();
        }

        if (result == 0)
        {
            is_exist = false;
        }

        return result;
    }

    bool IsExist() const
    {
        return is_exist;
    }
};


enum PartType
{
    Base = 0x0,
    ViewerFaced = 0x1,
    Barrel = 0x2,
    Rotor = 0x4,
    Subpart = 0x8,
    Emitter1 = 0x10,
    Emitter2 = 0x20,
    Emitter3 = 0x40,
    Emitter4 = 0x80
};


struct Part
{
    enum AnimatinType
    {
        Looped,
        TwoWay,
        Single,
        Lift
    };

    int num_verices;
    Array<Vertex> vertices;
    uint8 backTrackDepth;
    PartType type;
    String<> texture_file_name;
    Array<GFace> faces;
    Animations animations;
    AnimatinType animation_type;
    Point3D offset;
    double rise_angle;

    GenericGeometry *node;                         // Здесь геометрия данной части

    void Read(FileReader &, File &file);
    void ReadVertices(FileReader &);
    void ReadTextureInfo(FileReader &);
    void ReadFaces(FileReader &);
    void ReadAnimations(FileReader &, File &);

    GenericGeometry *CreateGeometry(int number) const;

    String<> NamePartType(PartType) const;

    void Destroy();
};


struct PartNode
{
    int               id;
    Array<Part *>     parts;            // На первый элемент цепляем детей
    PartNode *parent;
    Array<PartNode *> children;

    PartNode(int _id, Part *part = nullptr, PartNode *_parent = nullptr)
    {
        parts.AppendArrayElement(part);
        id = _id;
        parent = _parent;
        if (parent != nullptr)
        {
            parent->children.AppendArrayElement(this);
        }
    }

    ~PartNode()
    {
        Destroy();
    }

    void ConnectAllNodesToParent();

    void Destroy();
};


// Здесь хранится всё, что нужно для визуализации модели и анимации
struct GeoData2150
{
    Array<Part> parts;

    PartNode *partsTree = nullptr;

    bool first_calculate_frames = true;

    int num_frames = 0;

    int NumFrames();

    void Read(FileReader &, File &);

    Node *CreateModel();

    PartNode *CreatePartsTree();

    void Destroy();
};


struct ModelTemplate
{
    bool matrix[4][4];
    int16 flag;
};


struct MLight
{
    Vector3D value;
    ColorRGB color;
};


struct MSpotLight : public MLight
{
    float length;
    int direction;
    float width;
    float U3;
    float tilt;
    float ambience;
};


struct MOmniLight : public MLight
{
    float radius;
};


struct FlagRotations
{
    bool rot[4][4];
};


struct TemplateDetails
{
    int16 sectionHeights[4][4];
    uint8 sectionFlags[4][4];
    ModelTemplate sectionsRotations[4];
    FlagRotations sectionFlagRotations[4];
};


struct Slot
{
    int      id;
    Vector3D position;
    double   direction;
    uint8    flag;

    void WriteInfo(File &out_txt);

    bool IsValid() const
    {
        return position.x > -128.0f;
    }
};


struct Slots
{
    Slot turrets[4];            // бащня?
    Slot barrelMuzzels[4];      // стволы?
    Slot turretMuzzels[4];      // башенные дула?
    Slot headlights[4];         // фары?
    Slot omnilights[4];         // 
    Slot unloadPoints[4];       //
    Slot hitSpots[4];           // 
    Slot smokeSpots[4];         // пятна дыма?
    Slot unknown[4];
    Slot chimneys[2];           // дымоходы?
    Slot smokeTraces[2];        // дымовые следы?
    Slot exhausts[2];           // выхлопы?
    Slot keelTraces[2];         // 
    Slot interfacePivot;        // 
    Slot centerPivot;           //
    Slot productionSpotStart;   //
    Slot productionSpotEnd;     //
    Slot landingSpot;           // место посадки?
};


struct Boundaries
{
    float minX;
    float minY;

    float maxX;
    float maxY;
};


class Model2150 : public Node
{
public:

    Model2150(pchar name);

    ~Model2150()
    {
        geometry.Destroy();
    }

    static void Destroy();

    // Преобразует в модель C4 Engine
    Node *CreateModel();

    bool IsModel() const
    {
        return is_model;
    }

    // Сохранить в формате .mdl
    void SaveToFileMDL();

    Slots _slots;

private:

    static Array<Model2150 *> models;

    String<> file_name;

    ModelTemplate templ;

    TemplateDetails templateDetails;

    uint8 building_frames = 0;
    uint8 action_frames = 0;
    uint8 movement_frames = 0;
    uint8 looped_frames = 0;

    Array<Vector3D> mountPoints;
    Array<MSpotLight> spotLights;
    Array<MOmniLight> omniLights;

    Boundaries boundaries;

    GeoData2150 geometry;

    int mesh_type;
    int mesh_sub_type;

    bool ReadMeshFile(pchar name);

    Vector3D ReadVector3D(FileReader &);
    MSpotLight ReadSpotLight(FileReader &);
    MOmniLight ReadOmniLight(FileReader &);
    ColorRGB ReadColor(FileReader &);
    TemplateDetails ReadTemplateDetails(FileReader &);
    ModelTemplate ReadModelTemplate(FileReader &);
    Slots ReadModelSlots(FileReader &, File &out_txt);
    Slot ReadSlot(int id, FileReader &);
    void ReadSlots(int num, Slot *first, FileReader &, pchar name, File &out_txt);

    bool is_model = false;
};
