// 2022/09/05 08:47:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Canvas.h"
#include <map>


Canvas *Canvas::self = nullptr;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    self = this;

    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

    SetMinClientSize({ 100, 100 });
    SetClientSize(100, 100);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxBitmap bitmap(GetClientSize());
    wxMemoryDC memDC;

    memDC.SelectObject(bitmap);

    memDC.SetBrush(*wxWHITE_BRUSH);
    memDC.SetPen(wxPen(wxColor(0, 0, 0)));

    memDC.DrawRectangle(GetClientRect());

    DrawTimeScale(memDC);

    DrawAllSensors(memDC);

    memDC.SetPen(wxPen(wxColor(0, 0, 0)));

    memDC.DrawText("Тестовая строка", 1, 0);

    memDC.SelectObject(wxNullBitmap);

    wxPaintDC dc(this);

    dc.DrawBitmap(bitmap, 0, 0);

    Update();
}


void Canvas::DrawTimeScale(wxMemoryDC &)
{
}


void Canvas::SetSizeArea(int width, int height)
{
    SetMinClientSize({ width, height } );
    SetClientSize({ width, height });

    Refresh();
}


void Canvas::DrawAllSensors(wxMemoryDC &)
{
}


void Canvas::DrawTextOnBackground(wxMemoryDC &dc, pchar text, int x, int y, int width, int height)
{
    dc.DrawRectangle({ x, y }, { width, height });
    dc.DrawText(text, { x + 1, y + 1 });
}
