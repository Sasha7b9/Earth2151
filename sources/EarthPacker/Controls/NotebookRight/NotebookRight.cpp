// 2022/09/25 14:30:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/NotebookRight/NotebookRight.h"
#include "Controls/Canvas.h"


NotebookRight *NotebookRight::self = nullptr;


NotebookRight::NotebookRight(wxWindow *parent) : Notebook(parent)
{
    self = this;

    AddPage(new Canvas(this), "Info");
}
