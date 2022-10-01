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
    BeginScene(*wxWHITE_BRUSH, wxPen(wxColor(0, 0, 0)));

    DrawText(1, 0, "Тестовая строка");

    EndScene();
}


void Canvas::BeginScene(const wxBrush &brush, const wxPen &pen)
{
    bitmap = new wxBitmap(GetClientSize());
    memDC = new wxMemoryDC();

    memDC->SelectObject(*bitmap);

    SetBrush(brush);

    SetPen(pen);

    memDC->GradientFillLinear(GetClientRect(), brush.GetColour(), brush.GetColour());
}


void Canvas::DrawText(int x, int y, const wxString &text)
{
    memDC->DrawText(text, x, y);
}


void Canvas::EndScene()
{
    memDC->SelectObject(wxNullBitmap);

    wxPaintDC dc(this);

    dc.DrawBitmap(*bitmap, 0, 0);

    delete memDC;
    delete bitmap;
}


void Canvas::SetBrush(const wxBrush &brush)
{
    memDC->SetBrush(brush);
}


void Canvas::SetPen(const wxPen &pen)
{
    memDC->SetPen(pen);
}


void Canvas::DrawTextOnBackground(wxMemoryDC &dc, pchar text, int x, int y, int width, int height)
{
    dc.DrawRectangle({ x, y }, { width, height });
    dc.DrawText(text, { x + 1, y + 1 });
}
