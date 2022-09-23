// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"
#include "Display/Diagram/Canvas.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


/*
*   Виджет с графиком
*/


class Diagram : public wxPanel
{
public:

    Diagram(wxWindow *parant, TypeMeasure::E);

    void SetSizeArea(int, int);

private:

    Canvas *canvas = nullptr;

public:

    class Pool : public wxPanel
    {
    public:

        static Pool *Create(wxWindow *);

        static Pool *self;

        void SetSizeArea(int width, int height);

        void UpdateArea();

    private:

        Pool(wxWindow *parent);

        Diagram *pool[TypeMeasure::Count];
    };
};
