// 2022/09/25 14:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/NotebookLeft/NotebookLeft.h"
#include "Controls/NotebookLeft/PageDirectory.h"


NotebookLeft::NotebookLeft(wxWindow *parent) : Notebook(parent)
{
    new PageDirectory(this);

    AddPage(PageDirectory::self, "Directory");
}
