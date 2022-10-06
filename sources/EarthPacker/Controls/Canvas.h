// 2022/09/05 08:47:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   Здесь непосредственно отрисовка
*/


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    static Canvas *self;

protected:

    void BeginScene(const wxBrush &, const wxPen &);

    void SetBrush(const wxBrush &);

    void SetPen(const wxPen &);

    void DrawText(int, int, const wxString &);

    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawVLine(int x, int y1, int y2);
    void DrawHLine(int y, int x1, int x2);

    void EndScene();

private:

    wxBitmap *bitmap;
    wxMemoryDC *memDC;
};
