// 2025/03/15 11:37:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Tunnels.h"
#include "Objects/World/Landscape.h"
#include "Graphics/Primitives.h"


Tunnels::Tunnels(HeapBuffer &lnd, File &file_txt, int width, int height) : tunnels(this)
{
    /*
        1-й октет :             2-й октет - варианты :

        0 - нейтральный         00 01 02 03
        1 - АС                  14 15 16 17
        2 - EA                  28 29 2A 2B
        3 - пришельцы           3C 3D 3E 3F

        Текстуры

        TunnelAllowed       1 шт            разрешено строительство
        TunnelForbidden     1 шт            запрещено строительство
        TunnelCeiling       8 х 8 шт        потолок
        TunnelFloor         8 х 8 шт        пол
        TunnelWall          8 х 8 шт        стены
        В каждой текстуре с 64-мя изображениями есть 16 текстур для каждой фракции. В редакторе используется только 4.
        12 нельзя задать с помощью редактора
    */

    TexTunnels2150::Init();

    tunnels.SetSize(width, height);

    file_txt.WriteString("\n **************** Tunnels ***************************");

    int pointer = lnd.pointer_read;

    bool flag = false;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8 tunnel = 0;

            lnd.ReadData(&tunnel, 1);

            if (tunnel != 0 && tunnel != 0x40)
            {
                flag = true;
            }
        }
    }

    if (flag)
    {
        lnd.pointer_read = pointer;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                uint8 tunnel = 0;

                lnd.ReadData(&tunnel, 1);

                tunnels.SetTunnel(x, Y_FROM_LND(y) - 1, tunnel);

                if (tunnel == 0x40)                 // Здесь ничего нет
                {
                    file_txt.WriteText("   ");
                }
                else
                {
                    file_txt.WriteText(Text::Format("%02X ", tunnel));
                }
            }
            file_txt.WriteString("|");
        }
    }
    else
    {
        file_txt.WriteString("None");
    }

    tunnels.CreateTriangleMesh();

    CreateGeometry();
}


Tunnels::~Tunnels()
{
    tunnels.Destroy();
}


void Tunnels::Tuns::CreateTriangleMesh()
{
    float h = 100.0f;

    for (int x = 0; x < GetNumColumns(); x++)
    {
        for (int y = 0; y < GetNumRows(); y++)
        {
            if (Get(x, y) != 0x40)
            {
                AddPlaneH(x, y, h, h, h, h);

                float upY = t->UpY(y);
                float downY = t->DownY(y);
                float rightX = t->RightX(x);
                float leftX = t->LeftX(x);

                if (y < GetNumRows() - 1 && Get(x, y + 1) == 0x40)
                {
                    AddPlaneV(x, y, { leftX, upY }, { rightX, upY }, h);                // Up
                }

                if (Get(x + 1, y) == 0x40)
                {
                    AddPlaneV(x, y, { rightX, upY }, { rightX, downY }, h);             // Right
                }

                if (y > 0 && Get(x, y - 1) == 0x40)
                {
                    AddPlaneV(x, y, { rightX, downY }, { leftX, downY }, h);            // Down
                }

                if (Get(x - 1, y) == 0x40)
                {
                    AddPlaneV(x, y, { leftX, downY }, { leftX, upY }, h);               // Left
                }
            }
        }
    }
}


void Tunnels::CreateGeometry()
{
    GeometrySurface *surface = new GeometrySurface();

    for (int x = 1; x < tunnels.GetNumColumns() - 1; x++)
    {
        for (int y = 1; y < tunnels.GetNumRows() - 1; y++)
        {
            AppendTriangles(surface, tunnels.GetTriangles(x, y));
        }
    }

    surface->texcoordCount = 1;
    surface->surfaceFlags = kSurfaceValidNormals;

    List<GeometrySurface> surfaceList;
    surfaceList.AppendListElement(surface);

    const List<GeometrySurface> *surfaceTable = &surfaceList;

    MaterialObject *material = new MaterialObject();

    material->materialFlags = kMaterialAlphaTest;

    Texture *text = TexTunnels2150::GetTexture();

    if (text)
    {
        DiffuseTextureAttribute *attr = new DiffuseTextureAttribute(text);
        attr->SetTextureBlendMode(kTextureBlendPrimaryAlpha);
        material->AddAttribute(attr);
    }

    Array<MaterialObject *> materialArray;
    materialArray.AppendArrayElement(material);

    GenericGeometry *geometry = new GenericGeometry(1, &surfaceTable, materialArray);

    GenericGeometryObject *object = geometry->GetObject();

    object->SetGeometryRenderStage(kGeometryRenderStageTransparentEffect);

    object->SetGeometryBlendMode(kGeometryBlendModeAccumulate);

    object->geometryFlags &= ~kGeometryCastShadows;

    material->Release();

    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(geometry);

    delete surface;
}


void Tunnels::CreateGeometrySpheres()
{
    for (int y = 0; y < tunnels.GetNumRows(); y++)
    {
        for (int x = 0; x < tunnels.GetNumColumns(); x++)
        {
            if (tunnels.Get(x, y) != 0x40)
            {
                Point3D point{ (float)x, (float)y, 10.0f };

                Node *sphere = Sphere::CreateNew(point, 0.5f, Color::yellow, "");

                TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(sphere);
            }
        }
    }
}


void Tunnels::AppendTriangles(GeometrySurface *surface, const Array<Triangle> &triangles)
{
    for (int triangle = 0; triangle < triangles.GetArrayElementCount(); triangle++)
    {
        GeometryPolygon *polygon(new GeometryPolygon());

        const Triangle &tri = triangles[triangle];

        for (int i = 0; i < 3; i++)
        {
            GeometryVertex *vertex(new GeometryVertex());
            vertex->position = tri.ver[i];
            vertex->normal.Set(tri.norm[i].x, tri.norm[i].y, tri.norm[i].z);
            vertex->texcoord[0] = tri.textcoord[i];

            polygon->vertexList.AppendListElement(vertex);
        }

        surface->polygonList.AppendListElement(polygon);
    }
}


void Tunnels::Tuns::AddPlaneH(int x, int y, float hA, float hB, float hC, float hD)
{
    TexTile tex;

    bool append = TexTunnels2150::GetTile(tex);

    const Point2D tex1[3] = { tex._00, tex._01, tex._10 };

    AppendTriangle_LRLX_UUDY(x, y, { hA, hB, hD }, tex1, append);

    const Point2D tex2[3] = { tex._10, tex._01, tex._11 };

    AppendTriangle_RRLX_UDDY(x, y, { hB, hC, hD }, tex2, append);
}


void Tunnels::Tuns::AddPlaneV(int x, int y, const Point2D &p1, const Point2D &p2, float h)
{
    TexTile tex;

    TexTunnels2150::GetTile(tex);

    const Point2D tex1[3] = { tex._00, tex._01, tex._10 };

    const Point3D points1[3] =
    {
        { p1.x, p1.y, h + 1.0f, },
        { p1.x, p1.y, h },
        { p2.x, p2.y, h + 1.0f }
    };

    AppendTriangle(x, y, points1, tex1);

    const Point2D tex2[3] = { tex._10, tex._01, tex._11 };

    const Point3D points2[3] =
    {
        { p2.x, p2.y, h + 1.0f },
        { p1.x, p1.y, h },
        { p2.x, p2.y, h}
    };

    AppendTriangle(x, y, points2, tex2);
}


void Tunnels::Tuns::AppendTriangle_LRLX_UUDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { t->LeftX(x),  t->UpY(y),   p[0] },
            { t->LeftX(x),  t->DownY(y), p[2] },
            { t->RightX(x), t->UpY(y),   p[1] }
        };

        AppendTriangle(x, y, points, tex);
    }
}


void Tunnels::Tuns::AppendTriangle_RRLX_UDDY(int x, int y, const Point3D &p, const Point2D tex[3], bool append)
{
    if (append)
    {
        const Point3D points[3] =
        {
            { t->RightX(x), t->UpY(y),   p[0] },
            { t->LeftX(x),  t->DownY(y), p[2] },
            { t->RightX(x), t->DownY(y), p[1] }
        };

        AppendTriangle(x, y, points, tex);
    }
}


void Tunnels::Tuns::AppendTriangle(int x, int y, const Point3D points[3], const Point2D tex[3])
{
    AppendTriangle(x, y, { points, tex });
}
