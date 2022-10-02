// 2022/10/01 09:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


PageInfo *PageInfo::self = nullptr;


PageInfo::PageInfo(wxWindow *parent) : Canvas(parent)
{
    self = this;

    Bind(wxEVT_PAINT, &PageInfo::OnPaintEvent, this);
}


void PageInfo::OnPaintEvent(wxPaintEvent &)
{
    BeginScene(*wxWHITE_BRUSH, wxPen(wxColor(0, 0, 0)));

    DrawDescription();

    EndScene();
}


void PageInfo::DrawDescription()
{
    if (!description.IsValid())
    {
        DrawText(1, 0, "File is not supported");

        return;
    }
}


void PageInfo::SetDescriptionFile(const DescriptionFile &_description)
{
    description = _description;

    Refresh();
}
