// 2022/09/25 14:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/NotebookLeft/NotebookLeft.h"
#include "Packer/Packer.h"


NotebookLeft::NotebookLeft(wxWindow *parent) : Notebook(parent)
{
    pageDirectory = new PageDirectory(this);

    AddPage(pageDirectory, "Directory");

    Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &NotebookLeft::OnFileSelected, this);
    Bind(wxEVT_DIRCTRL_FILEACTIVATED, &NotebookLeft::OnFileActivated, this);
}


void NotebookLeft::OnFileSelected(wxTreeEvent &event)
{
    pageDirectory->GetPath(event.GetItem());
}


void NotebookLeft::OnFileActivated(wxTreeEvent &event)
{
    Packer::ProcessFile(pageDirectory->GetPath(event.GetItem()));
}
