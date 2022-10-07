// 2022/09/05 08:47:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Canvas.h"
#include <map>


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
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


void Canvas::DrawLine(int x1, int y1, int x2, int y2) const
{
    memDC->DrawLine(x1, y1, x2, y2);
}


void Canvas::DrawVLine(int x, int y1, int y2) const
{
    memDC->DrawLine(x, y1, x, y2);
}


void Canvas::DrawHLine(int y, int x1, int x2) const
{
    memDC->DrawLine(x1, y, x2, y);
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
