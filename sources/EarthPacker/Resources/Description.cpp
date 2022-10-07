// 2022/10/07 09:33:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Description.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


int Description::DrawCell(const PageInfo *page, int x, int y, int width, const wxString &text) const
{
    page->DrawVLine(x + width, y, y + PageInfo::PIXELS_IN_LINE);

    page->DrawText(x + 3, y + 2, text);

    return x + width;
}


int Description::DrawCell(const PageInfo *page, int x, int y, int width, int value) const
{
    return DrawCell(page, x, y, width, wxString::Format("%d", value));
}


int Description::DrawCell(const PageInfo *page, int x, int y, int width, const std::vector<uint8> &vec, int num_elements) const
{
    int result = x + width;

    page->DrawVLine(x + width, y, y + PageInfo::PIXELS_IN_LINE);

    if ((int)vec.size() < num_elements)
    {
        num_elements = (int)vec.size();
    }

    for (int i = 0; i < num_elements; i++)
    {
        page->DrawText(x + 3, y + 2, wxString::Format("%02X", vec[i]));
        x += 15;
    }

    return result;
}
