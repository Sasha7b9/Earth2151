// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "PanelMap.h"
#include "Settings.h"
#include "Interface/Controls/Button.h"
#include "Utils/Math.h"
#include "Interface/Widgets/GameCursor.h"
#include "Cameras.h"
#include "Objects/World/Landscape.h"
#include "Objects/World/Water.h"
#include "GameWorld.h"


PanelMap *ThePanelMap = nullptr;


PanelMap::PanelMap()
    : PanelWindow(SET::GUI::MAP::SIZE()),
    Global<PanelMap>(ThePanelMap)
{

    SetMovementMutator(Point2D((float)SET::GUI::MAP::VIEW::X(), (float)SET::GUI::MAP::VIEW::Y()),
        Point2D((float)-SET::GUI::MAP::WIDTH(), (float)SET::GUI::MAP::VIEW::Y()),
        Vector2D(-SET::GUI::PANEL::SPEED(), 0.0f));

    mutator = new PanelMapSprocket(this);
    AddSprocket(mutator);

    TheMouse->AddObserver(&observer);
}


PanelMap::~PanelMap()
{
    delete mutator;
}


void PanelMap::Destroy()
{
    SAFE_DELETE(ThePanelMap);
}


void PanelMap::HandleObserver(MouseObservable *, ::Type type)
{
    if ((uint)type == (uint)Mouse::Event::RightChanged ||
        ((uint)type == (uint)Mouse::Event::PositionChanged && TheMouse->RightIsPressed()))
    {
        if (!UnderMouse())
        {
            return;
        }

        Point3D position = GetWorldPosition();
        Point2D positionMouse = TheMouse->GetPosition();

        Point3D coordWorld = mutator->CoordMapToWorld({positionMouse.x - position.x, positionMouse.y - position.y});

        TheCameraRTS->SetPointFocus(coordWorld);
    }
}


void PanelMap::PreprocessWidget()
{
    PanelWindow::PreprocessWidget();
}


void PanelMap::HandleHideShow(Widget *, const WidgetEventData *)
{
    Toggle();
}


PanelMapSprocket::PanelMapSprocket(PanelMap *panel_) : Sprocket(kMutatorPanelMap), panel(panel_)
{

}


void PanelMap::ClearMap()
{
    mutator->Clear();
}


void PanelMapSprocket::Clear()
{
    panel->SetColorBackground(Color::white);
    panel->Clear();
    panel->EndPaint();

    need_build = true;
}


void PanelMapSprocket::MoveSprocket()
{
    if (!GameWorld::Current() || !LANDSCAPE)
    {
        return;
    }

    Landscape *landscape = GameWorld::Current()->landscape;

    if (need_build)
    {
        need_build = false;

        SAFE_DELETE(buffer);

        buffer = new Canvas(panel);

        int sizeMapX = landscape->GetNumColumns();
        int sizeMapY = landscape->GetNumRows();

        float sizePanelX = buffer->GetWidgetSize().x;
        float sizePanelY = buffer->GetWidgetSize().y;

        float scaleX = (float)(sizePanelX - 2) / (float)(sizeMapX);
        float scaleY = (float)(sizePanelY - 2) / (float)(sizeMapY);

        scale = scaleX < scaleY ? scaleX : scaleY;

        Point2D sizeRect(scale * (float)(sizeMapX)-1.0F, scale * (float)(sizeMapY)-1.0F);
        leftTop = Point2D(1.0F, 1.0F);

        if (scaleX < scaleY)
        {
            leftTop.y = sizePanelY / 2 - sizeRect.y / 2;
        }
        else
        {
            leftTop.x = sizePanelX / 2 - sizeRect.x / 2;
        }

        buffer->Clear();

        for (int row = 0; row < sizeMapY; row++)
        {
            for (int col = 0; col < sizeMapX; col++)
            {
                Point2D coord{ (float)col, (float)(landscape->GetNumRows() - row - 1) };

                float height_l = landscape->GetHeightApproximately(coord);
                float height = height_l;
                float heigth_w = 0.0f;
                bool water_valid = false;

                Water *water = GameWorld::Current()->landscape->water;

                if (water->GetLevel(coord, &heigth_w))
                {
                    if (heigth_w > height)
                    {
                        height = heigth_w;          // \todo Здесь нужно проверять, что высота воды больше высоты ландшафта, но так почему-то не всю воду рисует
                        water_valid = true;
                    }
                }

                float min = landscape->MinHeight();

                if (water->IsExist() && water->GetLevelMin() > min)
                {
                    min = water->GetLevelMin();
                }

                float range = (landscape->MaxHeight() - min);

                if (water_valid)
                {
                    buffer->SetColorBrush(Color::blue);
                }
                else
                {
                    float g = (height - min) / range * 1.5f;

                    if (g > 1.0f)
                    {
                        g = 1.0f;
                    }
                    else if (g < 0.0f)
                    {
                        g = 0.0f;
                    }

                    buffer->SetColorBrush(ColorRGBA(0.0F, g, 0.0F));
                }
                if (scale > 1.0F)
                {
                    float x = leftTop.x + (float)(col)*scale;
                    float y = leftTop.y + (float)(row)*scale;

                    buffer->FillRegion(x, y, scale, scale);
                }
                else
                {
                    float x = leftTop.x + (float)(col)*scale;
                    float y = leftTop.y + (float)(row)*scale;
                    buffer->DrawPoint(x, y);
                }
            }
        }
    }

    panel->CopyFrom(buffer);

    Point2D points[4] = {
        {0, 0},
        {SET::WINDOW::SIZE().x, 0},
        {SET::WINDOW::SIZE().x, SET::WINDOW::SIZE().y},
        {0, SET::WINDOW::SIZE().y}
    };
    Point3D point;

    panel->SetColorBrush(Color::white);

    if (TheCameraRTS->GetIntersectionPlaneZ(points[0], &point))
    {
        Point3D point1;
        if (TheCameraRTS->GetIntersectionPlaneZ(points[1], &point1))
        {
            panel->DrawLine(CoordWorldToMap(point), CoordWorldToMap(point1));
            Point3D point2;
            if (TheCameraRTS->GetIntersectionPlaneZ(points[2], &point2))
            {
                panel->DrawLine(CoordWorldToMap(point1), CoordWorldToMap(point2));
                Point3D point3;
                if (TheCameraRTS->GetIntersectionPlaneZ(points[3], &point3))
                {
                    panel->DrawLine(CoordWorldToMap(point2), CoordWorldToMap(point3));
                    panel->DrawLine(CoordWorldToMap(point3), CoordWorldToMap(point));
                }
            }
        }
    }

    panel->EndPaint();
}

Point2D PanelMapSprocket::CoordWorldToMap(const Point3D &coord)
{
    return Point2D(leftTop.x + coord.x * scale, panel->GetWidgetSize().y - ((coord.y + leftTop.y) + 1.0f) * scale);
}

Point3D PanelMapSprocket::CoordMapToWorld(const Point2D &coord)
{
    return Point3D((coord.x - leftTop.x) / scale, (panel->GetWidgetSize().y - (coord.y - leftTop.y) - 1.0f) / scale, 0.0f);
}

void PanelMapSprocket::ResetSprocket()
{
    Sprocket::ResetSprocket();
}

Sprocket *PanelMapSprocket::Replicate() const
{
    return new PanelMapSprocket(*this);
}