// 2025/03/15 11:36:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/HeapBuffer.h"


class Tunnels
{
public:

    Tunnels(HeapBuffer &, File &, int width, int height);

    ~Tunnels();

private:

    // Описывает элементарный треугольник для построения геометрической сетки
    struct Triangle
    {
        Triangle(const Point3D p[3], const Point2D tex[3])
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

            for (int i = 0; i < 3; i++)
            {
                textcoord[i] = tex[i];
            }
        }

        Point3D  ver[3];
        Vector3D norm[3];
        Point2D  textcoord[3];
    };

    struct Cell
    {
        Array<Triangle> triangles;
        uint8 tunnel;
    };

    struct Tuns
    {
        Tuns(Tunnels *_t) : t(_t) { }
        Tunnels *t;
        Array<Array<Cell>> rows;

        void SetTunnel(int x, int y, uint8 tunnel)
        {
            if (y >= 0)
            {
                rows[y][x].tunnel = tunnel;
            }
        }

        void SetSize(int width, int height)
        {
            rows.SetArrayElementCount(height);

            for (int i = 0; i < rows.GetArrayElementCount(); i++)
            {
                rows[i].SetArrayElementCount(width);
            }
        }

        int GetNumRows() const
        {
            return rows.GetArrayElementCount();
        }

        int GetNumColumns() const
        {
            return rows[0].GetArrayElementCount();
        }

        uint8 Get(int x, int y) const
        {
            return rows[y][x].tunnel;
        }

        void CreateTriangleMesh();

        const Array<Triangle> &GetTriangles(int x, int y)
        {
            return rows[y][x].triangles;
        }

        void AddPlaneH(int x, int y, float hA, float hB, float hC, float hD);

        void AddPlaneV(int x, int y, const Point2D &p1, const Point2D &p2, float h);

        void AppendTriangle_LRLX_UUDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);

        void AppendTriangle_RRLX_UDDY(int x, int y, const Point3D &, const Point2D tex[3], bool append);

        void AppendTriangle(int x, int y, const Point3D p[3], const Point2D tex[3]);

        void AppendTriangle(int x, int y, const Triangle &triang)
        {
            rows[y][x].triangles.AppendArrayElement(triang);
        }

        void Destroy()
        {
            for (int i = 0; i < rows.GetArrayElementCount(); i++)
            {
                rows[i].PurgeArray();
            }

            rows.PurgeArray();
        }
    };

    Tuns tunnels;

    void CreateGeometry();

    void CreateGeometrySpheres();

    void AppendTriangles(GeometrySurface *, const Array<Triangle> &);

    float LeftX(int x)
    {
        return float(x) - 0.5f;
    }

    float UpY(int y)
    {
        return float(y) + 0.5f;
    }

    float DownY(int y)
    {
        return float(y) - 0.5f;
    }

    float RightX(int x)
    {
        return float(x) + 0.5f;
    }
};
