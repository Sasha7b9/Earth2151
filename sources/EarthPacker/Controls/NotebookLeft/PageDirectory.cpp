// 2022/09/23 13:48:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/NotebookLeft/PageDirectory.h"
#include "Packer/Packer.h"


PageDirectory *PageDirectory::self = nullptr;


PageDirectory::PageDirectory(wxWindow *parent) : wxGenericDirCtrl(parent)
{
    self = this;

    wxWindowBase::SetMinClientSize({ 200, 100 });
    SetClientSize({ 200, 100 });

    ExpandPath("C:/Temp");

    Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &PageDirectory::OnFileSelected, this);
    Bind(wxEVT_DIRCTRL_FILEACTIVATED, &PageDirectory::OnFileActivated, this);
}

void PageDirectory::OnFileSelected(wxTreeEvent &event)
{
    GetPath(event.GetItem());
}


void PageDirectory::OnFileActivated(wxTreeEvent &event)
{
    Packer::ProcessFile(PageDirectory::self->GetPath(event.GetItem()));
}
