// 2022/10/01 09:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


PageInfo *PageInfo::self = nullptr;


PageInfo::PageInfo(wxWindow *parent) : Canvas(parent)
{
    self = this;

    Bind(wxEVT_PAINT, &PageInfo::OnPaintEvent, this);
    Bind(wxEVT_SIZE, &PageInfo::OnSizeEvent, this);
    Bind(wxEVT_SCROLL_LINEDOWN, &PageInfo::OnScrollEvent, this);
}


void PageInfo::OnPaintEvent(wxPaintEvent &)
{
    BeginScene(*wxWHITE_BRUSH, wxPen(wxColor(0, 0, 0)));

    DrawDescription();

    EndScene();
}


void PageInfo::OnSizeEvent(wxSizeEvent &event)
{
    MoveScrollBar();

    event.Skip();
}


void PageInfo::OnScrollEvent(wxScrollEvent &event)
{
    int i = 0;
}


void PageInfo::DrawDescription()
{
    for (size_t i = 0; i < description.size(); i++)
    {
        DrawText(1, i * PIXELS_IN_LINE, description[i]);
    }
}


void PageInfo::SetDescriptionFile(const DescriptionFile &_description)
{
    description = _description;

    ResetScrollBar();

    Refresh();
}


void PageInfo::ResetScrollBar()
{
    if (PIXELS_IN_LINE * description.size() > (uint)GetClientSize().GetHeight())
    {
        SetScrollbar(wxSB_VERTICAL, 0, GetClientSize().GetHeight() / PIXELS_IN_LINE, PIXELS_IN_LINE * description.size());
    }
    else
    {
        SetScrollbar(wxSB_VERTICAL, 0, 100, 100);
    }
}


void PageInfo::MoveScrollBar()
{

}
