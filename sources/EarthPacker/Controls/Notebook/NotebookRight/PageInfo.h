// 2022/10/01 09:05:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Canvas.h"
#include "Packer/DescriptionFile.h"


class PageInfo : public Canvas
{
public:
    PageInfo(wxWindow *parent);

    static PageInfo *self;

    void SetDescriptionFile(const DescriptionFile &);

private:

    static const int PIXELS_IN_LINE = 18;

    DescriptionFile description;

    void OnPaintEvent(wxPaintEvent &);

    void OnSizeEvent(wxSizeEvent &);

    void DrawDescription();

    // Вызывается при установке нового DescriptionFile
    void ResetScrollBar();

    // Вызывается при изменении размера
    void MoveScrollBar();
};
