// 2022/09/25 14:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/NotebookLeft.h"
#include "Controls/Notebook/NotebookLeft/PageDirectory.h"


NotebookLeft *NotebookLeft::self = nullptr;


NotebookLeft::NotebookLeft(wxWindow *parent) : Notebook(parent)
{
    self = this;

    wxBookCtrlBase::AddPage(new PageDirectory(this), "Directory");
}


void NotebookLeft::DeletePageRaw(wxWindow *window)
{
    size_t num_pages = GetPageCount();

    for (size_t i = 0; i < num_pages; i++)
    {
        if (GetPage(i) == window)
        {
            DeletePage(i);
            break;
        }
    }
}
