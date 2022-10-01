// 2022/09/05 08:47:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Canvas.h"
#include <map>


Canvas *Canvas::self = nullptr;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    self = this;

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
