// 2025/02/14 22:34:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/GUI.h"
#include "Interface/Widgets/GameCursor.h"
#include "Interface/Panels/PanelBottom.h"


GUI *TheGUI = nullptr;

Widget *GUI::trackedWidget = nullptr;
Point2D GUI::mouse_pos{ 0,0 };
Point2D GUI::widget_pos{ 0, 0 };

List<Widget> movabled;


GUI::GUI() : Widget(), Global<GUI>(TheGUI)
{
    new PanelBottom();

    new GameCursor();
}


GUI::~GUI()
{
    Hide();

    delete TheGameCursor;
    delete ThePanelBottom;
}


void GUI::Show()
{
    TheInterfaceMgr->AddWidget(ThePanelMap);
    TheInterfaceMgr->AddWidget(ThePanelMain);
    TheInterfaceMgr->AddWidget(ThePanelBottom);
    TheInterfaceMgr->AddWidget(TheGameCursor);
}


void GUI::Hide()
{
    TheInterfaceMgr->RemoveWidget(TheGameCursor);
    TheInterfaceMgr->RemoveWidget(ThePanelBottom);;
    TheInterfaceMgr->RemoveWidget(ThePanelMap);
    TheInterfaceMgr->RemoveWidget(ThePanelMain);
}


void GUI::MouseLeftIsPress()
{
    if (!TheGameCursor)
    {
        return;
    }

    mouse_pos = TheGameCursor->position;

    for (Widget *widget : movabled)
    {
        Point3D pos = widget->GetWorldPosition();
        Vector2D size = widget->GetWidgetSize();

        if (mouse_pos.x > pos.x && mouse_pos.x < (pos.x + size.x) &&
            mouse_pos.y > pos.y && mouse_pos.y < (pos.y + size.y))
        {
            trackedWidget = widget;
            widget_pos = widget->GetWidgetPosition();
            break;
        }
    }
}


void GUI::MouseLeftIsRelease()
{
    trackedWidget = nullptr;
}


bool GUI::UnderMouse() const
{
    return ThePanelMap->UnderMouse() || ThePanelMain->UnderMouse();
}
