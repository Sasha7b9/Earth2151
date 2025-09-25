// 2025/03/16 12:33:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Water.h"
#include "Objects/World/Landscape.h"
#include "Clock.h"


Water::~Water()
{
    cells.PurgeArray();

    for (int i = 0; i < waters.GetArrayElementCount(); i++)
    {
        waters[i].PurgeArray();
    }

    waters.PurgeArray();
}


Water::Water(HeapBuffer &lnd, File &file_txt, int w, int h)
{
    waters.SetArrayElementCount(h);

    for (int i = 0; i < h; i++)
    {
        waters[i].SetArrayElementCount(w);

        for (int x = 0; x < w; x++)
        {
            waters[i][x].valid = false;
        }
    }

//    uint time = UCOUNT_MS;

    file_txt.WriteString("\n **************** Water ***************************");

    uint level = 0;

    lnd.ReadData(&level, 4);

    level_sea = level;

    file_txt.WriteString(Text::Format("Level sea : %f", level_sea));

    Landscape *landscape = GameWorld::Current()->landscape;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            uint16 water = 0;

            lnd.ReadData(&water, 2);

            if (water != 0 &&
                water != 0x2000
                && water > 0xF8)
            {
                float height = ((water & 0x1FFF) - level_sea) / 256.0f;

                if (height < level_min)
                {
                    level_min = height;
                }

                cells.AppendArrayElement(WaterCell{ x, landscape->YfromLND(y), height });

                waters[landscape->YfromLND(y)][x].value = height;
                waters[landscape->YfromLND(y)][x].valid = true;
                is_exist = true;
            }

            if (x < 160)
            {
                if (water == 0)
                {
                    file_txt.WriteText("     ");
                }
                else
                {
                    file_txt.WriteText(Text::Format("%04X ", water));
                }
            }
        }

        file_txt.WriteString("|");
    }

    CreateGeometry();

//    LOG_WRITE("Time create water %u ms", UCOUNT_MS - time);
}


static GeometryVertex *CreateVertex(const Point3D &point)
{
    GeometryVertex *vertex = new GeometryVertex();

    vertex->position = point;

    return vertex;
}


void Water::CreateGeometry()
{
    GeometrySurface *surface = new GeometrySurface();

    for (int i = 0; i < cells.GetArrayElementCount(); i++)
    {
        float dx = -0.5f;
        float dy = 0.5f;

        WaterCell &cell = cells[i];

        GeometryPolygon *poligon = new GeometryPolygon();

        float h = cell.height;

        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx,        cell.y + dy, h }));
        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx + 1.0f, cell.y + dy, h}));
        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx,        cell.y + dy - 1.0f, h }));

        surface->polygonList.AppendListElement(poligon);

        poligon = new GeometryPolygon();

        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx,        cell.y + dy - 1.0f, h }));
        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx + 1.0f, cell.y + dy, h }));
        poligon->vertexList.AppendListElement(CreateVertex({ cell.x + dx + 1.0f, cell.y + dy - 1.0f, h }));

        surface->polygonList.AppendListElement(poligon);
    }

    surface->texcoordCount = 1;
    surface->surfaceFlags = kSurfaceValidNormals;

    List<GeometrySurface> surfaceList;
    surfaceList.AppendListElement(surface);

    const List<GeometrySurface> *surfaceTable = &surfaceList;

    MaterialObject *material = new MaterialObject();

    material->materialFlags = kMaterialTwoSided;

    DiffuseAttribute *diff = new DiffuseAttribute(Color::blue);
    material->AddAttribute(diff);

    Array<MaterialObject *> materialArray;
    materialArray.AppendArrayElement(material);

    GenericGeometry *geometry = new GenericGeometry(1, &surfaceTable, materialArray);

    geometry->GetObject()->geometryFlags &= ~kGeometryCastShadows;

    material->Release();

    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(geometry);

    delete surface;
} 


bool Water::GetLevel(const Point2D &coord, float *height)
{
    int x = (int)(coord.x + 0.5f);
    int y = (int)(coord.y + 0.5f);

    *height = waters[y][x].value;

    return waters[y][x].valid;
}


float Water::GetLevelMin()
{
    return level_min;
}
