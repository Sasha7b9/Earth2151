// 2025/02/27 20:18:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Files/Model2150.h"
#include "Utils/StringUtils.h"
#include "Graphics/Primitives.h"
#include "Game/Files/TexMesh2150.h"
#include "Earth2151.h"
#include <cstring>
#include <bitset>


Array<Model2150 *> Model2150::models;


static float DataToFloat(uint8 *data)
{
    float *result = (float *)data;

    return *result;
}


static int16 DataToInt16(uint8 *data)
{
    int16 *result = (int16 *)data;

    return *result;
}


static Vector3D ReadVector3D(FileReader &reader)
{
    Vector3D vector;

    reader.ReadBuffer(&vector, sizeof(vector));

    vector.y = -vector.y;

    return vector;
}


static Matrix4D ReadMatrix4D(FileReader &reader)
{
    Matrix4D matrix;

    uint size = sizeof(matrix);

    reader.ReadBuffer(&matrix, size);

    return matrix;
}


static Transform3D ReadTransform3D(FileReader &reader)
{
    Transform3D transform;

    reader.ReadBuffer(&transform, sizeof(transform));

    return transform;
}


Model2150::Model2150(pchar name) : Node()
{
    is_model = ReadMeshFile(name);

    models.AppendArrayElement(this);
}


void Model2150::Destroy()
{
    for (int i = 0; i < models.GetArrayElementCount(); i++)
    {
        delete models[i];
    }

    models.PurgeArray();
}


bool Model2150::ReadMeshFile(pchar _name)
{
    file_name = _name;

    String<> name_file = RESOURCE_PATH("meshes/") + _name;

    File file;

    if (file.OpenFile(name_file, kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR_HI("Can't open file %s", name_file.c_str());

        return false;
    }

    File file_txt;

    file_txt.OpenFile(name_file + ".txt", kFileCreate);

    float value1 = 1.0f;
    float value0 = 0.0f;

    uint hex1 = 0;
    uint hex0 = 0;

    std::memcpy(&hex1, &value1, 4);
    std::memcpy(&hex0, &value0, 4);

    hex0 = 0xBF000000;

    std::memcpy(&value0, &hex0, 4);

    uint8 uid_header[8] = { '\0' };

    {
        // Файлы, извлечённые с помощью EaathTool, имеют перед "MESH" какой-то придаток.
        // Пропускаем его

        for (int i = 0; i < 32; i++)
        {
            file.ReadFile(uid_header, 1);

            if (uid_header[0] == 0x4d)
            {
                file.ReadFile(uid_header + 1, 1);
                if (uid_header[1] == 0x45)
                {
                    file.ReadFile(uid_header + 2, 6);
                    break;
                }
            }
        }

        if (uid_header[0] != 0x4d)
        {
            LOG_WRITE("\"%s\" : Not mesh file", _name);

            return false;
        }
    }

    uint8 uid_header_pattern[8] = { 0x4d, 0x45, 0x53, 0x48, 0x01, 0x00, 0x00, 0x00 };

    if (std::memcmp(uid_header, uid_header_pattern, 8) != 0)
    {
        LOG_ERROR_HI("Not correct header for file \"%s\"", _name);

        return false;
    }

    FileReader reader(&file);

    mesh_type = reader.ReadInt();

    if (mesh_type != 0)      // Mesh type. Есть файлы с типом 1. Возможно, это файлы эффектов
    {
//        LOG_ERROR("Not correct type mesh, file \"%s\", type %d", _name, mesh_type);

        return false;
    }

    templ = ReadModelTemplate(reader);

    building_frames = reader.ReadByte();

    file_txt.WriteString(Text::Format("building frames : %d", building_frames));

    action_frames = reader.ReadByte();

    file_txt.WriteString(Text::Format("action frames : %d", action_frames));

    movement_frames = reader.ReadByte();

    file_txt.WriteString(Text::Format("movement frames : %d", movement_frames));

    looped_frames = reader.ReadByte();

    file_txt.WriteString(Text::Format("looped frames : %d", looped_frames));

    reader.ReadUInt();

    // EarthMeshReader.cs:68

    for (int i = 0; i < 4; i++)
    {
        Vector3D vector = ReadVector3D(reader);

        mountPoints.AppendArrayElement(vector);
    }

    // EarthMeshReader.cs:69

    for (int i = 0; i < 4; i++)
    {
        MSpotLight spot = ReadSpotLight(reader);

        spotLights.AppendArrayElement(spot);
    }

    // EarthMeshReader.cs:70

    for (int i = 0; i < 4; i++)
    {
        MOmniLight omni = ReadOmniLight(reader);

        omniLights.AppendArrayElement(omni);
    }

    // EarthMeshReader.cs:71

    templateDetails = ReadTemplateDetails(reader);

    // EarthMeshReader.cs:72

    _slots = ReadModelSlots(reader, file_txt);

    {
        // EarthMeshReader.cs:73

        boundaries.maxY = reader.ReadInt16();
        boundaries.minY = reader.ReadInt16();
        boundaries.maxX = reader.ReadInt16();
        boundaries.minX = reader.ReadInt16();
    }

    mesh_sub_type = reader.ReadInt();

    geometry.Read(reader, file_txt);

    return true;
}


Vector3D Model2150::ReadVector3D(FileReader &reader)
{
    Vector3D result;

    reader.ReadBuffer(&result, sizeof(result));

    result.y = -result.y;

    return result;
}


MSpotLight Model2150::ReadSpotLight(FileReader &reader)
{
    MSpotLight res;

    res.value = ReadVector3D(reader);
    res.color = ReadColor(reader);
    res.length = reader.ReadFloat();
    res.direction = reader.ReadInt();
    res.width = reader.ReadFloat();
    res.U3 = reader.ReadFloat();
    res.tilt = reader.ReadFloat();
    res.ambience = reader.ReadFloat();

    return res;
}


MOmniLight Model2150::ReadOmniLight(FileReader &reader)
{
    MOmniLight res;

    res.value = ReadVector3D(reader);
    res.color = ReadColor(reader);
    res.radius = reader.ReadFloat();

    return res;
}


ColorRGB Model2150::ReadColor(FileReader &reader)
{
    float red = reader.ReadFloat();
    float green = reader.ReadFloat();
    float blue = reader.ReadFloat();

    return ColorRGB(red, green, blue);
}


ModelTemplate Model2150::ReadModelTemplate(FileReader &reader)
{
    // EarthMeshReader.cs:117

    std::bitset<16> bitarray;

    reader.ReadBuffer(&bitarray, 2);

    ModelTemplate res;

    for (int col = 3; col >= 0; col--)
    {
        for (int row = 3; row >= 0; row--)
        {
            res.matrix[row][3 - col] = bitarray[col * 4 + row];
        }
    }

    res.flag = reader.ReadInt16();

    return res;
}


TemplateDetails Model2150::ReadTemplateDetails(FileReader &reader)
{
    TemplateDetails res;

    // EarthMeshReader.cs:151

    {
        // EarthMeshReader.cs:190

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                res.sectionHeights[row][col] = reader.ReadInt16();
            }
        }
    }

    {
        // EarthMeshReader.cs:205

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                res.sectionFlags[row][col] = reader.ReadByte();
            }
        }
    }

    {
        // EarthMeshReader.cs:153

        for (int i = 0; i < 4; i++)
        {
            res.sectionsRotations[i] = ReadModelTemplate(reader);
        }
    }

    {
        // EarthMeshReader.cs:220
        for (int rot = 0; rot < 4; rot++)
        {
            for (int i = 0; i < 4; i++)
            {
                int16 columnValue = reader.ReadInt16();
                uint8 upperByte = (uint8)(columnValue >> 8);
                uint8 lowerByte = (uint8)(columnValue & 0xFF);
                uint8 r0 = (uint8)(upperByte >> 4);
                uint8 r1 = (uint8)(upperByte & 0x0F);
                uint8 r2 = (uint8)(lowerByte >> 4);
                uint8 r3 = (uint8)(lowerByte & 0x0F);
                res.sectionFlagRotations[rot].rot[i][0] = r0;
                res.sectionFlagRotations[rot].rot[i][1] = r1;
                res.sectionFlagRotations[rot].rot[i][2] = r2;
                res.sectionFlagRotations[rot].rot[i][3] = r3;
            }
        }
    }

    return res;
}


Slots Model2150::ReadModelSlots(FileReader &reader, File &out_txt)
{
    Slots res;

    out_txt.WriteString("Slots:");

    ReadSlots(4, res.turrets, reader, "turrets", out_txt);
    ReadSlots(4, res.barrelMuzzels, reader, "barrelMuzzels", out_txt);
    ReadSlots(4, res.turretMuzzels, reader, "turretMuzzels", out_txt);
    ReadSlots(4, res.headlights, reader, "headlights", out_txt);
    ReadSlots(4, res.omnilights, reader, "omnilights", out_txt);
    ReadSlots(4, res.unloadPoints, reader, "unloadPoints", out_txt);
    ReadSlots(4, res.hitSpots, reader, "hitSpots", out_txt);
    ReadSlots(4, res.smokeSpots, reader, "smokeSpots", out_txt);
    ReadSlots(4, res.unknown, reader, "unknown", out_txt);
    ReadSlots(2, res.chimneys, reader, "chimneys", out_txt);
    ReadSlots(2, res.smokeTraces, reader, "smokeTraces", out_txt);
    ReadSlots(2, res.exhausts, reader, "exhausts", out_txt);
    ReadSlots(2, res.keelTraces, reader, "keelTraces", out_txt);
    ReadSlots(1, &res.interfacePivot, reader, "interfacePivot", out_txt);
    ReadSlots(1, &res.centerPivot, reader, "centerPivot", out_txt);
    ReadSlots(1, &res.productionSpotStart, reader, "productionSpotStart", out_txt);
    ReadSlots(1, &res.productionSpotEnd, reader, "productionSpotEnd", out_txt);
    ReadSlots(1, &res.landingSpot, reader, "landingSpot", out_txt);

    return res;
}


Slot Model2150::ReadSlot(int id, FileReader &reader)
{
    float x = reader.ReadInt16() / 255.0f;
    float y = -reader.ReadInt16() / 255.0f;
    float z = reader.ReadInt16() / 255.0f;

    Vector3D vec{ x, y, z };

    return { id, vec, reader.ReadByte() / 255.0f * Math::pi * 2.0f, reader.ReadByte() };
}


void Model2150::ReadSlots(int num, Slot *slot, FileReader &reader, pchar name, File &out_txt)
{
    out_txt.WriteString(name);

    for (int i = 0; i < num; i++)
    {
        *slot = ReadSlot(i, reader);

        slot->WriteInfo(out_txt);

        slot++;
    }
}


void Slot::WriteInfo(File &out_txt)
{
    if (IsValid())
    {
        out_txt.WriteString(Text::Format("    id %d, pos %s, dir %f, flag 0x%02X", id, SU::Vec3toStr(position).c_str(), direction, flag));
    }
}


void GeoData2150::Read(FileReader &reader, File &file_txt)
{
    int num_part = 1;

    while (!reader.IsEOF())
    {
        Part part;

        file_txt.WriteString(Text::Format("             *** Load part %d ***", num_part++));

        part.Read(reader, file_txt);

        parts.AppendArrayElement(part);
    }
}


void GeoData2150::Destroy()
{
    for (int i = 0; i < parts.GetArrayElementCount(); i++)
    {
        parts[i].Destroy();
    }

    parts.PurgeArray();

    delete partsTree;
}


void PartNode::Destroy()
{
    for (int i = 0; i < children.GetArrayElementCount(); i++)
    {
        delete children[i];
    }

    children.PurgeArray();
}


int GeoData2150::NumFrames()
{
    if (first_calculate_frames)
    {
        first_calculate_frames = false;

        num_frames = 0;

        for (Part &part : parts)
        {
            if (part.animations.NumFrames() > num_frames)
            {
                num_frames = part.animations.NumFrames();
            }
        }
    }

    return num_frames;
}


PartNode *GeoData2150::CreatePartsTree()
{
    int currentID = 0;

    PartNode *node = new PartNode(currentID++, &parts[0]);

    for (int num_part = 1; num_part < parts.GetArrayElementCount(); num_part++)
    {
        Part *part = &parts[num_part];

        int skip = part->backTrackDepth;

        PartNode *parent = node;

        for (int i = 0; i < skip; i++)
        {
            parent = parent->parent;
        }

        if (part->type == PartType::Base)
        {
            node = parent;
            node->parts.AppendArrayElement(part);
        }
        else
        {
            node = new PartNode(currentID++, part, parent);
        }
    }

    while (node->parent != nullptr)
    {
        node = node->parent;
    }

    return node;
}


void Part::Read(FileReader &reader, File &file_txt)
{
    ReadVertices(reader);
    backTrackDepth = reader.ReadByte();
    file_txt.WriteString(Text::Format("backtrack %d", backTrackDepth).c_str());
    type = (PartType)reader.ReadByte();
    file_txt.WriteString(Text::Format("type part : %d", type));
    int16 flag_int16 = reader.ReadInt16();
    file_txt.WriteString(Text::Format("int16 : %d", flag_int16));
    ReadTextureInfo(reader);                // EarthMeshReader.cs:261
    ReadFaces(reader);
    ReadAnimations(reader, file_txt);
    animation_type = (AnimatinType)reader.ReadInt();
    offset = ReadVector3D(reader);
    file_txt.WriteString(Text::Format("Offset : %s", SU::Vec3toStr(offset).c_str()));
    rise_angle = ((double)reader.ReadByte() / 255.0) * 360;
    uint8 b0 = reader.ReadByte();                      // \todo unknown flag
    uint8 b1 = reader.ReadByte();                      // \todo unknown flag
    uint8 b2 = reader.ReadByte();                      // \todo unknown flag
    uint8 b3 = reader.ReadByte();                      // \tood unknown flag

    file_txt.WriteString(Text::Format("Unknown flags : %d, %d, %d, %d", b0, b1, b2, b3).c_str());
}


void Part::Destroy()
{
    vertices.PurgeArray();
    faces.PurgeArray();
}


void Part::ReadAnimations(FileReader &reader, File &file_txt)
{
    file_txt.WriteString("Animations:");

    int num_scales = reader.ReadInt();

    file_txt.WriteString(Text::Format("    num scales = %d", num_scales));

    for (int i = 0; i < num_scales; i++)
    {
        animations.scales.AppendArrayElement(ReadVector3D(reader));
    }

    int num_trans = reader.ReadInt();

    file_txt.WriteString(Text::Format("    num trans = %d", num_trans));

    for (int i = 0; i < num_trans; i++)
    {
        Vector3D vector = ReadVector3D(reader);
        animations.trans.AppendArrayElement(vector);
    }

    int num_rot = reader.ReadInt();

    file_txt.WriteString(Text::Format("    num rotations = %d", num_rot));

    for (int i = 0; i < num_rot; i++)
    {
        animations.rotations.AppendArrayElement(ReadTransform3D(reader));
    }
}


void Part::ReadVertices(FileReader &reader)
{
    num_verices = reader.ReadInt();
    int num_blocks = reader.ReadInt();

    for (int block = 0; block < num_blocks; block++)
    {
        uint8 data[160];

        reader.ReadBuffer(data, 160);

        for (int i = 0; i < 4; i++)
        {
            int idx = i * sizeof(float);

            float x = DataToFloat(data + idx + 0x00);
            float y = -DataToFloat(data + idx + 0x10);
            float z = DataToFloat(data + idx + 0x20);

            float normX = DataToFloat(data + idx + 0x30);
            float normY = -DataToFloat(data + idx + 0x40);
            float normZ = DataToFloat(data + idx + 0x50);

            float u = DataToFloat(data + idx + 0x60);
            float v = DataToFloat(data + idx + 0x70);

            int16 u1 = DataToInt16(data + i * sizeof(int16) + 0x90);
            int16 u2 = DataToInt16(data + i * sizeof(int16) + 0x98);

            Vertex vertex
            {
                {x, y, z},
                {normX, normY, normZ},
                {u, v},
                u1,
                u2
            };

            vertices.AppendArrayElement(vertex);
        }
    }
}


void Part::ReadTextureInfo(FileReader &reader)
{
    texture_file_name = reader.ReadString();
}


void Part::ReadFaces(FileReader &reader)
{
    int num_faces = reader.ReadInt();

    for (int i = 0; i < num_faces; i++)
    {
        GFace face;

        for (int v = 0; v < 4; v++)
        {
            face.index[v] = reader.ReadInt16();
        }

        faces.AppendArrayElement(face);
    }
}


Node *Model2150::CreateModel()
{
    return geometry.CreateModel();
}


Node *GeoData2150::CreateModel()
{
    Node *node = new Node();

    for (int i = 0; i < parts.GetArrayElementCount(); i++)
    {
        GenericGeometry *geometry = parts[i].CreateGeometry(i);

        parts[i].node = geometry;
    }

    partsTree = CreatePartsTree();

    partsTree->ConnectAllNodesToParent();

    node->AppendNewSubnode(partsTree->parts[0]->node);

    ModelController *controller = new ModelController();
    controller->SetModel(this);
    controller->main_node = partsTree->parts[0]->node;

    node->SetController(controller);

    return node;
}


void PartNode::ConnectAllNodesToParent()
{
    for (int i = 1; i < parts.GetArrayElementCount(); i++)
    {
        if (parts[0]->animations.IsExist())
        {
            parts[i]->node->SetNodePosition(Point3D::zero);
        }
        else
        {
            parts[i]->node->SetNodePosition(parts[i]->offset);
        }

        parts[0]->node->AppendNewSubnode(parts[i]->node);
    }

    if (parent)
    {
        parent->parts[0]->node->AppendNewSubnode(parts[0]->node);
    }

    for (PartNode *node : children)
    {
        node->ConnectAllNodesToParent();
    }
}


void ModelController::MoveController()
{
    if (model->NumFrames() == 0)
    {
        return;
    }

    if (UCOUNT_MS < time_prev_frame + speed_animation)
    {
        return;
    }

    time_prev_frame += speed_animation;

    int frame = counter++ / 1;

    if (frame == model->NumFrames())
    {
        counter = 0;
        frame = 0;
    }

    for (int i = 0; i < model->parts.GetArrayElementCount(); i++)
    {
        const Part &part = model->parts[i];

        if (!part.animations.IsExist())
        {
            continue;
        }

        Node *node = part.node;

        if (part.animations.rotations.GetArrayElementCount() > frame)
        {
            node->SetNodeRotation(part.animations.rotations[frame]);
        }

        if (part.animations.trans.GetArrayElementCount() > frame)
        {
            node->SetNodePosition(part.animations.trans[frame]);
        }
        else if(part.animations.trans.GetArrayElementCount() != 0)
        {
            node->SetNodePosition(part.animations.trans.GetLastElement());
        }
        else
        {
            node->SetNodePosition(part.offset);
        }

        node->InvalidateNode();

        node->UpdateTransform();

        Node *subnode = node;

        do
        {
            subnode->InvalidateNode();
            subnode->UpdateTransform();

            subnode = node->GetNextTreeNode(subnode);

        } while (subnode);
    }
}


void Vertex::ConvertToGeometryVertex(GeometryVertex *vertex)
{
    vertex->position = coord;

    vertex->normal.Set(normal.x, normal.y, normal.z);

    vertex->texcoord[0].x = tex.x;
    vertex->texcoord[0].y = tex.y;
}


GenericGeometry *Part::CreateGeometry(int num) const
{
    GeometrySurface *surface = new GeometrySurface();

    for (const GFace &face : faces)
    {
        GeometryPolygon *poligon = new GeometryPolygon();

        for (uint i = 0; i < 3; i++)
        {
            GeometryVertex *vertex = new GeometryVertex();

            int index = face.index[i];

            if (index < vertices.GetArrayElementCount())
            {
                vertices[index].ConvertToGeometryVertex(vertex);
            }

            poligon->vertexList.AppendListElement(vertex);
        }

        surface->polygonList.AppendListElement(poligon);
    }

    surface->texcoordCount = 1;
    surface->surfaceFlags = kSurfaceValidNormals;

    List<GeometrySurface> surfaceList;
    surfaceList.AppendListElement(surface);

    const List<GeometrySurface> *surfaceTable = &surfaceList;

    MaterialObject *material = new MaterialObject();

//    material->materialFlags = 0;

    material->materialFlags = kMaterialAlphaTest;

    Texture *text = TexMesh2150::GetTexture(texture_file_name.c_str(), 0);

    if (text)
    {
        DiffuseTextureAttribute *attr = new DiffuseTextureAttribute(text);
        material->AddAttribute(attr);
    }

    Array<MaterialObject *> materialArray;
    materialArray.AppendArrayElement(material);

    GenericGeometry *geometry = new GenericGeometry(1, &surfaceTable, materialArray);

//    geometry->GetObject()->geometryFlags |= kGeometryCastShadows | kGeometryRenderEffect;

    geometry->GetObject()->geometryFlags |= kGeometryCastShadows;

    geometry->SetRenderState(kGeometryRenderStagePrimary);

//    geometry->GetObject()->SetGeometryBlendMode(kGeometryBlendModeInterpolate);

    material->Release();

    geometry->SetNodeName(Text::Format("Node%02d%s", num + 1, NamePartType(type).c_str()).c_str());

    return geometry;
}


String<> Part::NamePartType(PartType _type) const
{
    String<> result;

    if (_type == PartType::Base)
    {
        result.AppendString("_Base");
    }
    if (_type & PartType::ViewerFaced)
    {
        result.AppendString("_ViewerFaced");
    }
    if (_type & PartType::Barrel)
    {
        result.AppendString("_Barrel");
    }
    if (_type & PartType::Subpart)
    {
        result.AppendString("_Subpart");
    }
    if (_type & PartType::Emitter1)
    {
        result.AppendString("_Emitter1");
    }
    if (_type & PartType::Emitter2)
    {
        result.AppendString("_Emitter2");
    }
    if (_type & PartType::Emitter3)
    {
        result.AppendString("_Emitter3");
    }
    if (_type & PartType::Emitter4)
    {
        result.AppendString("_Emitter4");
    }

    return result;
}


void Model2150::SaveToFileMDL()
{
    if (IsModel())
    {
        Node *node = CreateModel();

        // Если раскомментировать, то gizmo исчезает с модели
        {
            Model *modelC4 = new Model();

            modelC4->AppendNewSubnode(node);

            File file;

            if (file.OpenFile(RESOURCE_PATH("meshes/") + file_name + ".mdl", kFileCreate) == kFileOkay)
            {
                modelC4->PackWorldResource(file, kPackInitialize);

                file.CloseFile();
            }

            delete modelC4;
        }
    }
    else
    {
        LOG_ERROR_HI("%s is not mesh file", file_name.c_str());
    }
}
