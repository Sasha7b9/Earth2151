// 2022/10/01 09:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


PageInfo *PageInfo::self = nullptr;


PageInfo::PageInfo(wxWindow *parent) : Canvas(parent)
{
    self = this;

    Bind(wxEVT_PAINT, &PageInfo::OnPaintEvent, this);
    Bind(wxEVT_SIZE, &PageInfo::OnSizeEvent, this);
    Bind(wxEVT_SCROLLWIN_THUMBTRACK, &PageInfo::OnScrollEvent, this);

    scroll_bar.keeper = this;
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


void PageInfo::OnScrollEvent(wxScrollWinEvent &event)
{
    scroll_bar.SetPosition(event.GetPosition());
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

    scroll_bar.Reset();

    Refresh();
}


void PageInfo::ScrollBar::Reset()
{
    if (PIXELS_IN_LINE * keeper->description.Size() > keeper->GetClientSize().GetHeight())
    {
        keeper->SetScrollbar(wxSB_VERTICAL, SetPosition(0), GetThumb(), GetRange());
    }
    else
    {
        keeper->SetScrollbar(wxSB_VERTICAL, SetPosition(0), 100, 100);
    }
}


int PageInfo::ScrollBar::GetRange()
{
    return PIXELS_IN_LINE * keeper->description.Size();
}


int PageInfo::ScrollBar::GetThumb()
{
    return keeper->GetClientSize().GetHeight() / PIXELS_IN_LINE;
}


int PageInfo::ScrollBar::SetPosition(int pos)
{
    position = pos;

    keeper->SetScrollPos(wxSB_VERTICAL, position);

    return position;
};



void PageInfo::MoveScrollBar()
{

}
