// 2025/02/23 13:20:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Landscape.h"
#include "Graphics/PoolTextures.h"
#include "Earth2150/Files/TexLand2150.h"
#include "Utils/Math.h"
#include "GameWorld.h"
#include "Objects/World/Tunnels.h"
#include "Objects/World/Water.h"
#include "Clock.h"
#include <map>


namespace Landscape
{
    static int SIZE_SEGMENT = 64;                           // Ландшафт разделён на сегменты для ускорения загрузки (так можно загружать его потоками и потому
                                                            // что двигатель не может обрабатывать геометрию с более чем 65535 вершинами

    static Array<GeometryObject *> geometries;

    // Сюда складываются вектора нормалей по мере построения треугольников, чтобы во время построения ландшафта извлекать их усреднения для
    // получения сглаживания ландшафта

    struct StructNormals
    {
        StructNormals(int _x, int _y)
        {
            x = _x;
            y = _y;
        }

        int x;
        int y;

        int Value() const
        {
            return x * 1000 + y;
        }

        bool operator<(const StructNormals &b) const
        {
            return Value() < b.Value();
        }
    };

    template<class T>
    struct StorageNormals
    {
        std::map<T, Array<Vector3D>> normals;

        void Destroy()
        {
            for (auto &item : normals)
            {
                item.second.PurgeArray();
            }

            normals.clear();
        }

        int X(float x)
        {
            return (int)(x * 1000.0f);
        }

        int Y(float y)
        {
            return (int)(y * 1000.0f);
        }

        void Append(float x, float y, const Vector3D &normal)
        {
            T key{ X(x), Y(y) };

            auto it = normals.find(key);

            if (it == normals.end())
            {
                Array<Vector3D> array;

                normals.emplace(std::pair{ key , array });
            }

            it = normals.find(key);

            it->second.AppendArrayElement(normal);
        }

        Vector3D Get(float x, float y)
        {
            auto it = normals.find({ X(x), Y(y) });

            if (it != normals.end())
            {
                Vector3D sum = Vector3D::zero;

                for (int i = 0; i < it->second.GetArrayElementCount(); i++)
                {
                    Vector3D &vec = (it->second)[i];

                    sum += vec;;
                }

                return sum.Normalize();
            }

            return Vector3D::zero;
        }
    };

    static StorageNormals<StructNormals> storage_normals;

    // Описывает элементарный треугольник для построения геометрической сетки ландшафта
    struct Triangle
    {
        Triangle(const Point3D p[3],  const Point2D tex[3])
        {
            for (int i = 0; i < 3; i++)
            {
                ver[i] = p[i];
            }

            Vector3D vec01 = p[0] - p[1]; //-V525
            Vector3D vec02 = p[0] - p[2];
            Vector3D vec12 = p[1] - p[2];

            norm[0] = Cross(vec01, vec02);
            norm[1] = Cross(vec12, -vec01);
            norm[2] = Cross(-vec02, -vec12);

            storage_normals.Append(ver[0].x, ver[0].y, norm[0]);
            storage_normals.Append(ver[1].x, ver[1].y, norm[1]);
            storage_normals.Append(ver[2].x, ver[2].y, norm[2]);

            for (int i = 0; i < 3; i++)
            {
                textcoord[i] = tex[i];
            }
        }

        Point3D  ver[3];
        Vector3D norm[3];
        Point2D  textcoord[3];
    };

    struct ArrayLand
    {
        void SetSize(int width, int height);

        Point2D GetSize() const;

        void SetHeight(int x, int y, float height);

        float GetHeight(int x, int y) const
        {
            return rows[y][x].height;
        }

        Tile GetTile(int x, int y)
        {
            return rows[y][x].tile;
        }

        void SetTunnel(int x, int y, uint8);

        void SetTexture(int x, int y, uint8);

        uint8 GetTexture(int x, int y) const;

        void SetResource(int x, int y, uint8);

        void SetWater(int x, int y, float);

        int GetNumColumns() const
        {
            return rows[0].GetArrayElementCount();
        }

        int GetNumRows() const
        {
            return rows.GetArrayElementCount();
        }

        void AppendTriangle(int x, int y, const Triangle &triang)
        {
            rows[y][x].triangles.AppendArrayElement(triang);
        }

        const Array<Triangle> &GetTriangles(int x, int y)
        {
            return rows[y][x].triangles;
        }

        void SetCorner(int x, int y, int corner, const Point3D &coord)
        {
            rows[y][x].corners[corner] = coord;
        }

        void AddPlane1(int x, int y, float hA, float hB, float hC, float hD);
        void AddPlane2(int x, int y, float hA, float hB, float hC, float hD);

        void AppendTriangle_LRLX_UUDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_RRLX_UDDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_LRRX_UUDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_LRLX_UDDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle(int x, int y,  const Point3D p[3], const Point2D tex[3]);

    private:

        int width;
        int height;

    public:

        struct Cell
        {
            Triangle &GetTriangle(int i)
            {
                return triangles[i];
            }

            void Destroy()
            {
                triangles.PurgeArray();
            }

            float           height;
            uint8           texture;
            uint8           resource;
            float           water;
            Array<Triangle> triangles;      // Треугольники сетки, принадлежащие данной ячейке
            Point3D         corners[4];     // Через эти точки строим плоскость, покрывающую данную ячейку
            Tile            tile;           // Здесь хранятся типы ладшафта по углам
        };

        Array<Array<Cell>> rows;            // Это рядки. Рядок с номером (height - 1) соответствует рядку 0 в системе координат Earth 2150 (левый верхний угол карты)
                                            // Нумерация столбиков совпадает

        Cell &GetCell(int x, int y)
        {
            return rows[y][x];
        }

        void Destroy()
        {
            for (int i = 0; i < rows.GetArrayElementCount(); i++)
            {
                Array<Cell> &row = rows[i];

                for (int j = 0; j < row.GetArrayElementCount(); j++)
                {
                    row[j].Destroy();
                }

                row.PurgeArray();
            }

            rows.PurgeArray();
        }
    };

    static ArrayLand land;
    static float min_height = 1e3f;
    static float max_height = -1e3f;

    static float LeftX(int x)
    {
        return float(x) - 0.5f;
    }

    static float RightX(int x)
    {
        return float(x) + 0.5f;
    }

    static float UpY(int y)
    {
        return float(y) + 0.5f;
    }

    static float DownY(int y)
    {
        return float(y) - 0.5f;
    }

    static uint       typeEarth2150;        // Значение по смещению 0 из файла .lnd
    static String<>   fileName2150;         // Имя файла из файла .lnd
    static uint8      guid2150[16];
    static String<>   translate_mission;
    static int        type_texture;         // 0...7 в зависимости от времени года

    static void FillLnd(Level2150 &);

    // siz_size - размер ячейки хранения размера
    static String<> ReadString(int size_size, HeapBuffer &);

    // Рассчитывает треугольники для построения сетки ландшафта
    static void CreateTrianglesMesh();

    // Строит геометрию ландашфта по заранее рассчитанным треугольникам
    static void CreateGeometryLandscapeFromTriangles();

    // Создаёт один сегмент ландшафта
    static void CreateSegment(int x_segment, int y_segment);

    static void AppendTriangles(GeometrySurface *, const Array<Triangle> &);
}


void Landscape::Create(Level2150 &info)
{
    uint time = UCOUNT_MS;

    FillLnd(info);

    LOG_WRITE("time fill %u", UCOUNT_MS - time);

    time = UCOUNT_MS;

    TexLand2150::CreateTiles();

    LOG_WRITE("time tiles %u", UCOUNT_MS - time);

    time = UCOUNT_MS;

    CreateTrianglesMesh();

    LOG_WRITE("time create mesh %u", UCOUNT_MS - time);

    time = UCOUNT_MS;

    CreateGeometryLandscapeFromTriangles();

    LOG_WRITE("time create geometry %u", UCOUNT_MS - time);
}


void Landscape::Destroy()
{
    land.Destroy();

    geometries.PurgeArray();

    storage_normals.Destroy();

    Water::Destroy();
}


bool Landscape::IsCreated()
{
    return land.GetNumRows();
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

    LOG_WRITE("Landscape %d x %d", width, height);

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

    Tunnels::ReadTunnels(lnd, file_txt, width, height);

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

    Water::Create(lnd, file_txt, width, height);

    file_txt.WriteString("\n **************** Heights ***************************");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float h = (land.GetHeight(x, y) - Water::LevelSea()) / 256.0f;

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
        LOG_ERROR("Bad file");
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
                    land.AddPlane1(x, y, h0, h1, h2, h3);
                }
                else
                {
                    land.AddPlane2(x, y, h0, h1, h2, h3);
                }
            }
            else
            {
                if (h0 > height)    // Если инврертировать это условие, то исчезнут ступеньки на боках гор
                {
                    land.AddPlane2(x, y, h0, h1, h2, h3);
                }
                else
                {
                    land.AddPlane1(x, y, h0, h1, h2, h3);
                }
            }
        }
    }
}


void Landscape::ArrayLand::AddPlane1(int x, int y, float hA, float hB, float hC, float hD)
{
    TexTile tex;

    bool append = TexLand2150::GetTile(GetTile(x, y), tex);

    const Point2D tex1[3] = { tex._00, tex._01, tex._10 };

    land.AppendTriangle_LRLX_UUDY(x, y, { hA, hB, hD }, tex1, append);

    const Point2D tex2[3] = { tex._10, tex._01, tex._11 };

    land.AppendTriangle_RRLX_UDDY(x, y, { hB, hC, hD }, tex2, append);
}


void Landscape::ArrayLand::AddPlane2(int x, int y, float hA, float hB, float hC, float hD)
{
    TexTile tex;

    bool append = TexLand2150::GetTile(GetTile(x, y), tex);

    const Point2D tex1[3] = { tex._00, tex._11, tex._10 };

    land.AppendTriangle_LRRX_UUDY(x, y, { hA, hB, hC }, tex1, append);

    const Point2D tex2[3] = { tex._00, tex._01, tex._11 };

    land.AppendTriangle_LRLX_UDDY(x, y, { hA, hC, hD }, tex2, append);
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


void Landscape::ArrayLand::AppendTriangle(int x, int y, const Point3D points[3], const Point2D tex[3])
{
    AppendTriangle(x, y, { points, tex });
}


void Landscape::ArrayLand::AppendTriangle_LRLX_UUDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), UpY(y), p[1] }
        };

        AppendTriangle(x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_RRLX_UDDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { RightX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), DownY(y), p[1] }
        };

        AppendTriangle(x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_LRRX_UUDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { RightX(x), DownY(y), p[2] },
            { RightX(x), UpY(y), p[1] }
        };

        AppendTriangle(x, y, points, tex);
    }
}


void Landscape::ArrayLand::AppendTriangle_LRLX_UDDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { LeftX(x), UpY(y), p[0] },
            { LeftX(x), DownY(y), p[2] },
            { RightX(x), DownY(y), p[1] }
        };

        AppendTriangle(x, y, points, tex);
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
        LOG_ERROR("No intersection with terrain detected in %f:%f", (float)coord.x, (float)coord.y);
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
