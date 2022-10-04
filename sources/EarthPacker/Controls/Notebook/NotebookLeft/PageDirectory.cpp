// 2022/09/23 13:48:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/PageDirectory.h"
#include "Packer/Packer.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"
#include "Packer/Resources/ArchiveWD.h"


PageDirectory *PageDirectory::self = nullptr;


PageDirectory::PageDirectory(wxWindow *parent) : wxGenericDirCtrl(parent)
{
    self = this;

    wxWindowBase::SetMinClientSize({ 200, 100 });
    SetClientSize({ 200, 100 });

    wxGenericDirCtrl::ExpandPath("C:/Temp");

    Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &PageDirectory::OnFileSelected, this);
    Bind(wxEVT_DIRCTRL_FILEACTIVATED, &PageDirectory::OnFileActivated, this);
    Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &PageDirectory::OnRightClick, this);
}

void PageDirectory::OnFileSelected(wxTreeEvent &event) //-V2009
{
    wxString path = GetPath(event.GetItem());

    DescriptionFile description;

    Packer::GetDescriptionFile(path, description);

    PageInfo::self->SetDescriptionFile(description);
}


void PageDirectory::OnFileActivated(wxTreeEvent &event) //-V2009
{
    Packer::ProcessFile(PageDirectory::self->GetPath(event.GetItem()));
}


void PageDirectory::OnRightClick(wxTreeEvent &event)
{
    wxMenu menu;
    menu.Append(wxID_ANY, "Unpack");
    PopupMenu(&menu, event.GetPoint().x, event.GetPoint().y);

    event.Skip();
}
