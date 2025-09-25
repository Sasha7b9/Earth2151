// 2025/02/23 13:20:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Landscape.h"
#include "Graphics/PoolTextures.h"
#include "Game/Files/TexLand2150.h"
#include "Utils/Math.h"
#include "GameWorld.h"
#include "Objects/World/Tunnels.h"
#include "Objects/World/Water.h"
#include "Clock.h"
#include <map>



Landscape::Landscape(Level2150 &info, GameWorld *world)
{
    world->landscape = this;

    FillLnd(info);

    TexLand2150::CreateTiles();

    CreateTrianglesMesh();

    CreateGeometryLandscapeFromTriangles();
}


void Landscape::Create(Level2150 &info, GameWorld *world)
{
    new Landscape(info, world);
}


Landscape::~Landscape()
{
    land.Destroy();
    geometries.PurgeArray();
    storage_normals.Destroy();

    SAFE_DELETE(tunnels);

    SAFE_DELETE(water);
}


void Landscape::FillLnd(Level2150 &info)
{
    HeapBuffer &lnd = info.dataLND;

    lnd.ResetRead();

    lnd.ReadData(&typeEarth2150, 4);

    fileName2150 = ReadString(1, lnd);

    uint unknown = 0;

    lnd.ReadData(&unknown, 4);

    lnd.ReadData(guid2150, 16);

    int width = 0;
    int height = 0;

    lnd.ReadData(&width, 4);

    lnd.ReadData(&height, 4);

    land.SetSize(width, height);

//    LOG_WRITE("Landscape %d x %d", width, height);

    File file_txt;

    file_txt.OpenFile(info.file_name_lnd + ".txt", kFileCreate);

    file_txt.WriteString(Text::Format("Size : %d x %d", width, height));

    lnd.ReadData(&unknown, 4);

    translate_mission = ReadString(4, lnd);

    {                                                   // \todo Разные форматы, по крайней мере, в !112 и !113
        int pointer = lnd.pointer_read;

        lnd.ReadData(&unknown, 4);

        lnd.pointer_read = pointer;

        if (unknown < 256)
        {
            String<> name_texture = ReadString(4, lnd);

            type_texture = (name_texture[name_texture.GetStringLength() - 1] & 0x0F) - 1;
        }
        else
        {
            uint8 guid[16];

            lnd.ReadData(guid, 16);

            type_texture = guid[15];
        }
    }

    TexLand2150::SetType((TexLand2150::Type::E)type_texture);

    file_txt.WriteString(Text::Format("Type : %d", type_texture));

    for (int y = 0; y < height; y++)                                    // /
    {                                                                   // | Так идёт нумерация в файле lnd
        for (int x = 0; x < width; x++)                                 // /
        {
            int lev = 0;

            lnd.ReadData(&lev, 2);

            land.SetHeight(x, YfromLND(y), (float)lev);
        }
    }

    tunnels = new Tunnels(lnd, file_txt, width, height);

    file_txt.WriteString("\n **************** Textures ***************************");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8 texture = 0;

            lnd.ReadData(&texture, 1);

            land.SetTexture(x, YfromLND(y), texture & 0x07);

            file_txt.WriteText(Text::Format("%2X ", texture));
        }

        file_txt.WriteString("");
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8 resource = 0;

            lnd.ReadData(&resource, 1);

            land.SetResource(x, YfromLND(y), resource);
        }
    }

    water = new Water(lnd, file_txt, width, height);

    file_txt.WriteString("\n **************** Heights ***************************");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float h = (land.GetHeight(x, y) - water->LevelSea()) / 256.0f;

            if (h < min_height)
            {
                min_height = h;
            }
            if (h > max_height)
            {
                max_height = h;
            }

            land.SetHeight(x, y, h);

            file_txt.WriteText(Text::Format("%4d ", (int)(land.GetHeight(x, y) * 256.0f)));
        }

        file_txt.WriteString("|");
    }

    if (lnd.pointer != lnd.pointer_read)
    {
        LOG_ERROR_HI("Bad file");
    }
}


String<> Landscape::ReadString(int size_size, HeapBuffer &lnd)
{
    uint size = 0;

    lnd.ReadData(&size, size_size);

    HeapBuffer string(128);

    for (uint i = 0; i < size; i++)
    {
        char symbol = 0;

        lnd.ReadData(&symbol, 1);

        string.WriteData(&symbol, 1);
    }

    size = 0;

    string.WriteData(&size, 1);

    return String<>((char *)string.data);
}


void Landscape::ArrayLand::SetSize(int _width, int _height)
{
    width = _width;
    height = _height;

    rows.SetArrayElementCount(height);

    for (int i = 0; i < height; i++)
    {
        rows[i].SetArrayElementCount(width);
    }
}


Point2D Landscape::ArrayLand::GetSize() const
{
    if (rows.GetArrayElementCount())
    {
        return { (float)rows[0].GetArrayElementCount(), (float)rows.GetArrayElementCount() };
    }

    return { 0, 0 };
}


void Landscape::ArrayLand::SetHeight(int x, int y, float h)
{
    rows[y][x].height = h;
}


void Landscape::ArrayLand::SetTexture(int x, int y, uint8 texture)
{
    rows[y][x].texture = texture;

    rows[y][x].tile.s.tex00 = texture & 0x07;

    if (x > 0)
    {
        rows[y][x - 1].tile.s.tex10 = texture;
    }
    if (x > 0 && y > 0)
    {
        rows[y - 1][x - 1].tile.s.tex11 = texture;
    }
    if (y > 0)
    {
        rows[y - 1][x].tile.s.tex01 = texture;
    }
}


uint8 Landscape::ArrayLand::GetTexture(int x, int y) const
{
    return rows[y][x].texture;
}


void Landscape::ArrayLand::SetResource(int x, int y, uint8 resource)
{
    rows[y][x].resource = resource;
}

void Landscape::ArrayLand::SetWater(int x, int y, float water)
{
    rows[y][x].water = water;
}


void Landscape::CreateTrianglesMesh()
{
    // Модификация метода 3, которая всегда разрезает ячейку так, чтобы разрез шёл вдоль оси Z

    for (int x = 1; x < land.GetNumColumns() - 1; x++)
    {
        for (int y = 1; y < land.GetNumRows() - 1; y++)
        {
            float height = land.GetHeight(x, y);
            float h_up = land.GetHeight(x, y + 1);
            float h_up_r = land.GetHeight(x + 1, y + 1);
            float h_r = land.GetHeight(x + 1, y);
            float h_dn_r = land.GetHeight(x + 1, y - 1);
            float h_dn = land.GetHeight(x, y - 1);
            float h_dn_l = land.GetHeight(x - 1, y - 1);
            float h_l = land.GetHeight(x - 1, y);
            float h_up_l = land.GetHeight(x - 1, y + 1);

            /*    0        1   0        1
                    +----+       +----+
                    +   /|       +\   |
                    +  / |       + \  |
                    + /  |       +  \ |
                    +/   |       +   \|
                    +----+       +----+
                  3        2   3        2
            */

            float h0 = (height + h_l + h_up_l + h_up) / 4.0f;
            float h1 = (height + h_up + h_up_r + h_r) / 4.0f;
            float h2 = (height + h_r + h_dn_r + h_dn) / 4.0f;
            float h3 = (height + h_dn + h_dn_l + h_l) / 4.0f;

            if (h1 > h0)
            {
                if (h1 < height)   // Если инврертировать это условие, то исчезнут ступеньки на боках гор
                {
                    land.AddPlane1(storage_normals, x, y, h0, h1, h2, h3);
                }
                else
                {
                    land.AddPlane2(storage_normals, x, y, h0, h1, h2, h3);
                }
            }
            else
            {
                if (h0 > height)    // Если инврертировать это условие, то исчезнут ступеньки на боках гор
                {
                    land.AddPlane2(storage_normals, x, y, h0, h1, h2, h3);
                }
                else
                {
                    land.AddPlane1(storage_normals, x, y, h0, h1, h2, h3);
                }
            }
        }
    }
}


void Landscape::ArrayLand::AddPlane1(StorageNormals<StructNormals> &stn, int x, int y, float hA, float hB, float hC, float hD)
{
    TexTile tex;

    bool append = TexLand2150::GetTile(GetTile(x, y), tex);

    const Point2D tex1[3] = { tex._00, tex._01, tex._10 };

    AppendTriangle_LRLX_UUDY(stn, x, y, { hA, hB, hD }, tex1, append);

    const Point2D tex2[3] = { tex._10, tex._01, tex._11 };

    AppendTriangle_RRLX_UDDY(stn, x, y, { hB, hC, hD }, tex2, append);
}


void Landscape::ArrayLand::AddPlane2(StorageNormals<StructNormals> &stn, int x, int y, float hA, float hB, float hC, float hD)
{
    TexTile tex;

    bool append = TexLand2150::GetTile(GetTile(x, y), tex);

    const Point2D tex1[3] = { tex._00, tex._11, tex._10 };

    AppendTriangle_LRRX_UUDY(stn, x, y, { hA, hB, hC }, tex1, append);

    const Point2D tex2[3] = { tex._00, tex._01, tex._11 };

    AppendTriangle_LRLX_UDDY(stn, x, y, { hA, hC, hD }, tex2, append);
}


void Landscape::CreateGeometryLandscapeFromTriangles()
{
    int num_segments_x = land.GetNumColumns() / SIZE_SEGMENT;

    if (land.GetNumColumns() % SIZE_SEGMENT)
    {
        num_segments_x++;
    }

    int num_segments_y = land.GetNumRows() / SIZE_SEGMENT;

    if (land.GetNumRows() % SIZE_SEGMENT)
    {
        num_segments_y++;
    }

    for (int x = 0; x < num_segments_x; x++)
    {
        for (int y = 0; y < num_segments_y; y++)
        {
            CreateSegment(x, y);
        }
    }
}


void Landscape::CreateSegment(int x_segment, int y_segment)
{
    int first_x = x_segment * SIZE_SEGMENT;
    int last_x = first_x + SIZE_SEGMENT;

    if (last_x > land.GetNumColumns())
    {
        last_x = land.GetNumColumns();
    }

    int first_y = y_segment * SIZE_SEGMENT;
    int last_y = first_y + SIZE_SEGMENT;

    if (last_y > land.GetNumRows())
    {
        last_y = land.GetNumRows();
    }

    GeometrySurface *surface = new GeometrySurface();

    for (int x = first_x; x < last_x; x++)
    {
        for (int y = first_y; y < last_y; y++)
        {
            AppendTriangles(surface, land.GetTriangles(x, y));
        }
    }

    surface->texcoordCount = 1;
    surface->surfaceFlags = kSurfaceValidNormals;

    List<GeometrySurface> *surfaceList = new List<GeometrySurface>();
    surfaceList->AppendListElement(surface);
    const List<GeometrySurface> *surfaceTable = surfaceList;

    MaterialObject *material = new MaterialObject();

    material->materialFlags = 0;

    Texture *texture = TexLand2150::GetCanvas()->GetTexture();

    DiffuseTextureAttribute *diff = new DiffuseTextureAttribute(texture);

    material->AddAttribute(diff);

    Array<MaterialObject *> materialArray;
    materialArray.AppendArrayElement(material);

    GenericGeometry *geometry = new GenericGeometry(1, &surfaceTable, materialArray);

    GeometryObject *object = geometry->GetObject();
    object->geometryFlags |= kGeometryCastShadows;

    geometries.AppendArrayElement(object);

    material->Release();

    delete surfaceList;

    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(geometry);
}


void Landscape::AppendTriangles(GeometrySurface *surface, const Array<Triangle> &triangles)
{
    for (int triangle = 0; triangle < triangles.GetArrayElementCount(); triangle++)
    {
        GeometryPolygon *polygon(new GeometryPolygon());

        const Triangle &tri = triangles[triangle];

        for (int i = 0; i < 3; i++)
        {
            GeometryVertex *vertex(new GeometryVertex());
            vertex->position = tri.ver[i];

            Vector3D normal = storage_normals.Get(tri.ver[i].x, tri.ver[i].y);

            vertex->normal.Set(normal.x, normal.y, normal.z);
            vertex->texcoord[0] = tri.textcoord[i];

            polygon->vertexList.AppendListElement(vertex);
        }

        surface->polygonList.AppendListElement(polygon);
    }
}


void Landscape::ArrayLand::AppendTriangle(StorageNormals<StructNormals> &stn, int x, int y, const Point3D points[3], const Point2D tex[3])
{
    AppendTriangle( x, y, { stn, points, tex });
}


void Landscape::ArrayLand::AppendTriangle_LRLX_UUDY(StorageNormals<StructNormals> &stn, int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), UpY(y), p[1] }
        };

        AppendTriangle(stn, x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_RRLX_UDDY(StorageNormals<StructNormals> &stn, int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { RightX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), DownY(y), p[1] }
        };

        AppendTriangle(stn, x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_LRRX_UUDY(StorageNormals<StructNormals> &stn, int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { RightX(x), DownY(y), p[2] },
            { RightX(x), UpY(y), p[1] }
        };

        AppendTriangle(stn, x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_LRLX_UDDY(StorageNormals<StructNormals> &stn, int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), DownY(y), p[1] }
        };

        AppendTriangle(stn, x, y, points, tex);
    }
}


Point2D Landscape::GetSize()
{
    return land.GetSize();
}


int Landscape::GetNumColumns()
{
    return land.GetNumColumns();
}


int Landscape::GetNumRows()
{
    return land.GetNumRows();
}


float Landscape::GetHeight(const Point2D &coord, Bivector3D *normal)
{
    GeometryHitData data;

    uint64 time = TheTimeMgr->GetMicrosecondCount();

    static int counter = 0;

    for (int i = 0; i < geometries.GetArrayElementCount(); i++)
    {
        counter++;

        if (geometries[i]->DetectCollision({ coord.x + 1e-2f, coord.y + 1e-2f, 100.0f }, { coord.x + 1e-2f, coord.y + 1e-2f, -100.0f }, 0.0f, &data))
        {
            time = TheTimeMgr->GetMicrosecondCount() - time;

//            LOG_WRITE("Time detection %llu us, %d iterations", time, i + 1);

            if (normal)
            {
                *normal = data.normal;
            }

            return data.position.z;
        }
    }

    static bool first = true;

    if (first)
    {
        first = false;
        LOG_ERROR_HI("No intersection with terrain detected in %f:%f", (float)coord.x, (float)coord.y);
    }

    return 25.0f;
}


float Landscape::GetHeightApproximately(const Point2D &coord)
{
    return land.GetHeight((int)(coord.x + 0.5f), (int)(coord.y + 0.5f));
}


Tile Landscape::GetTile(int x, int y)
{
    return land.GetTile(x, y);
}


int Landscape::YfromLND(int y)
{
    return land.GetNumRows() - y - 1;
}


float Landscape::MinHeight()
{
    return min_height;
}


float Landscape::MaxHeight()
{
    return max_height;
}
