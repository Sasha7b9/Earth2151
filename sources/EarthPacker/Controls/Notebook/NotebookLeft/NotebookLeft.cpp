// 2022/09/25 14:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/NotebookLeft.h"
#include "Controls/Notebook/NotebookLeft/PageDirectory.h"


NotebookLeft *NotebookLeft::self = nullptr;


NotebookLeft::NotebookLeft(wxWindow *parent) : Notebook(parent)
{
    self = this;

    AddPage(new PageDirectory(this), "Directory");
}
