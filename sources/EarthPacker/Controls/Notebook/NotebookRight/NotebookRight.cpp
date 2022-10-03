// 2022/09/25 14:30:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookRight/NotebookRight.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"


NotebookRight *NotebookRight::self = nullptr;


NotebookRight::NotebookRight(wxWindow *parent) : Notebook(parent)
{
    self = this;

    wxBookCtrlBase::AddPage(new PageInfo(this), "Info");
}
