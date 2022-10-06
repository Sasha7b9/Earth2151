// 2022/10/01 09:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


PageInfo *PageInfo::self = nullptr;


PageInfo::PageInfo(wxWindow *parent) : Canvas(parent)
{
    self = this;

    Bind(wxEVT_PAINT, &PageInfo::OnPaintEvent, this);
    Bind(wxEVT_SIZE, &PageInfo::OnSizeEvent, this);
    Bind(wxEVT_SCROLLWIN_LINEUP, &PageInfo::OnScrollLineUpEvent, this);
    Bind(wxEVT_SCROLLWIN_LINEDOWN, &PageInfo::OnScrollLineDownEvent, this);
    Bind(wxEVT_SCROLLWIN_THUMBTRACK, &PageInfo::OnScrollTrackEvent, this);
    Bind(wxEVT_MOUSEWHEEL, &PageInfo::OnMouseWheel, this);

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
    event.Skip();
}


void PageInfo::OnMouseWheel(wxMouseEvent &event)
{
    scroll_bar.MoveOnLines(event.GetWheelRotation() < 0 ? 5 : -5);

    Refresh();
}


void PageInfo::OnScrollTrackEvent(wxScrollWinEvent &event) //-V2009
{
    scroll_bar.SetPosition(event.GetPosition());

    Refresh();
}


void PageInfo::OnScrollLineUpEvent(wxScrollWinEvent &)
{
    scroll_bar.MoveOnLines(-1);

    Refresh();
}


void PageInfo::OnScrollLineDownEvent(wxScrollWinEvent &)
{
    scroll_bar.MoveOnLines(1);

    Refresh();
}


void PageInfo::DrawDescription()
{
    int first_line = scroll_bar.GetFirstLine();

    Canvas::DrawHLine(0, 0, GetClientSize().GetWidth() - 1);

    for (size_t i = first_line; i < description.size(); i++)
    {
        DrawLine(i * PIXELS_IN_LINE - first_line * PIXELS_IN_LINE, description[i]);
    }
}


void PageInfo::SetDescriptionFile(const DescriptionFileWD &_description)
{
    description = _description;

    scroll_bar.Reset();

    Refresh();
}


void PageInfo::ScrollBar::Reset()
{
    keeper->SetScrollbar(wxSB_VERTICAL, SetPosition(0), GetThumb(), GetRange());
}


int PageInfo::ScrollBar::GetRange()
{
    return PIXELS_IN_LINE * keeper->description.Size() + PIXELS_IN_LINE;
}


int PageInfo::ScrollBar::GetThumb()
{
    return keeper->GetClientSize().GetHeight();
}


int PageInfo::ScrollBar::SetPosition(int pos)
{
    position = pos;

    keeper->SetScrollPos(wxSB_VERTICAL, position);

    return position;
};


int PageInfo::ScrollBar::GetFirstLine()
{
    if (position == 0)
    {
        return 0;
    }

    return position / keeper->PIXELS_IN_LINE;
}


void PageInfo::ScrollBar::MoveOnLines(int num_lines)
{
    position += num_lines * keeper->PIXELS_IN_LINE;

    if (position < 0)
    {
        position = 0;
    }
    else if (position >= GetRange())
    {
        position = GetRange();
    }

    LOG_WRITE("range %d, position %d", GetRange(), position);

    SetPosition(position);
}


void PageInfo::DrawLine(int y, const DescInfo &desc)
{
    int width = GetClientSize().GetWidth();

    Canvas::DrawLine(0, y, 0, y + PIXELS_IN_LINE);
    Canvas::DrawLine(0, y + PIXELS_IN_LINE, width, y + PIXELS_IN_LINE);
    Canvas::DrawLine(width - 1, y, width - 1, y + PIXELS_IN_LINE);

    int x = DrawCell(0, y, 35, wxString::Format("%d", desc.num_line));

    x = DrawCell(x, y, 390, desc.name);

    x = DrawCell(x, y, 50, wxString::Format("%d", desc.size));

    wxString text_size = (desc.size != desc.decompressed_size) ? wxString::Format("%d", desc.decompressed_size) : "*";

    DrawCell(x, y, 55, text_size);
}


int PageInfo::DrawCell(int x, int y, int width, const wxString &text)
{
    Canvas::DrawVLine(x + width, y, y + PIXELS_IN_LINE);
    DrawText(x + 3, y + 2, text);

    return x + width;
}
