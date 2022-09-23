// 2022/09/05 08:47:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   ����� ��������������� ���������
*/


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent, int);

    void SetSizeArea(int, int);

private:

    void OnPaint(wxPaintEvent &);

    // ���������� ��������� �� ���� ��������
    void DrawAllSensors(wxMemoryDC &);

    void DrawTimeScale(wxMemoryDC &);

    void DrawTextOnBackground(wxMemoryDC &, pchar, int x, int y, int width, int height);

    int type;
};
