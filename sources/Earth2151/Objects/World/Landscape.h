// 2025/02/23 13:20:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Earth2150Tool.h"
#include "Game/Files/TexLand2150.h"
#include "GameWorld.h"
#include "Objects/World/Tunnels.h"
#include "Objects/World/Water.h"


// Переводит координату y из координт файла .lnd в наши координаты
#define Y_FROM_LND(y)   LANDSCAPE->YfromLND(y)

#define LANDSCAPE       GameWorld::Current()->landscape


class Landscape
{
public:

    static void Create(Level2150 &, GameWorld *);

    ~Landscape();

    Point2D GetSize();

    int GetNumColumns();

    int GetNumRows();

    float GetHeight(const Point2D &, Bivector3D *normal = nullptr);

    float GetHeightApproximately(const Point2D &);

    Tile GetTile(int x, int y);

    // Переводит координату y из координт файла .lnd в наши координаты
    int YfromLND(int);

    float MaxHeight();
    float MinHeight();

    Tunnels *tunnels = nullptr;

    Water *water = nullptr;

private:

    static const int SIZE_SEGMENT = 64;     // Ландшафт разделён на сегменты для ускорения загрузки (так можно загружать его потоками и потому
                                            // что двигатель не может обрабатывать геометрию с более чем 65535 вершинами

    Array<GeometryObject *> geometries;

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

    StorageNormals<StructNormals> storage_normals;

    // Описывает элементарный треугольник для построения геометрической сетки ландшафта
    struct Triangle
    {
        Triangle(StorageNormals<StructNormals> &stn, const Point3D p[3], const Point2D tex[3])
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

            stn.Append(ver[0].x, ver[0].y, norm[0]);
            stn.Append(ver[1].x, ver[1].y, norm[1]);
            stn.Append(ver[2].x, ver[2].y, norm[2]);

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

        void AddPlane1(StorageNormals<StructNormals> &, int x, int y, float hA, float hB, float hC, float hD);
        void AddPlane2(StorageNormals<StructNormals> &, int x, int y, float hA, float hB, float hC, float hD);

        void AppendTriangle_LRLX_UUDY(StorageNormals<StructNormals> &, int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_RRLX_UDDY(StorageNormals<StructNormals> &, int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_LRRX_UUDY(StorageNormals<StructNormals> &, int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle_LRLX_UDDY(StorageNormals<StructNormals> &, int x, int y, const Point3D &, const Point2D tex[3], bool append);
        void AppendTriangle(StorageNormals<StructNormals> &, int x, int y, const Point3D p[3], const Point2D tex[3]);

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

    ArrayLand land;
    float min_height = 1e3f;
    float max_height = -1e3f;

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

    Landscape(Level2150 &, GameWorld *);

    uint       typeEarth2150;        // Значение по смещению 0 из файла .lnd
    String<>   fileName2150;         // Имя файла из файла .lnd
    uint8      guid2150[16];
    String<>   translate_mission;
    int        type_texture;         // 0...7 в зависимости от времени года

    void FillLnd(Level2150 &);

    // siz_size - размер ячейки хранения размера
    String<> ReadString(int size_size, HeapBuffer &);

    // Рассчитывает треугольники для построения сетки ландшафта
    void CreateTrianglesMesh();

    // Строит геометрию ландашфта по заранее рассчитанным треугольникам
    void CreateGeometryLandscapeFromTriangles();

    // Создаёт один сегмент ландшафта
    void CreateSegment(int x_segment, int y_segment);

    void AppendTriangles(GeometrySurface *, const Array<Triangle> &);
};
