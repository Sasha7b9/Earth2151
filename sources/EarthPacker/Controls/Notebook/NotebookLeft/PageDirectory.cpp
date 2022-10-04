// 2022/09/23 13:48:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/PageDirectory.h"
#include "Packer/Packer.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"
#include "Packer/Resources/ArchiveWD.h"
#include "Controls/Notebook/NotebookLeft/NotebookLeft.h"
#include "Controls/Notebook/NotebookLeft/PageInfoWD.h"


PageDirectory *PageDirectory::self = nullptr;


enum
{
    ID_POPUP_UNPACK = wxID_HIGHEST + 1,
    ID_POPUP_VIEW
};


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

    Packer::ArchiveWD arch(path);

    arch.GetDescription(description);

    PageInfo::self->SetDescriptionFile(description);
}


void PageDirectory::OnFileActivated(wxTreeEvent &event) //-V2009
{
    Packer::ProcessFile(GetPath(event.GetItem()));
}


void PageDirectory::OnRightClick(wxTreeEvent &event)
{
    SelectPath(GetPath(event.GetItem()));

    if (!Packer::ArchiveWD::IsCorrectFile(GetPath(event.GetItem())))
    {
        return;
    }

    wxMenu menu;

    menu.Append(ID_POPUP_VIEW, "View");
    Bind(wxEVT_MENU, &PageDirectory::OnMenuView, this, ID_POPUP_VIEW);

    menu.Append(ID_POPUP_UNPACK, "Unpack");
    Bind(wxEVT_MENU, &PageDirectory::OnMenuUnpack, this, ID_POPUP_UNPACK);

    PopupMenu(&menu, event.GetPoint());

    event.Skip();
}


void PageDirectory::OnMenuUnpack(wxCommandEvent &)
{
    Packer::ProcessFile(GetPath());
}


void PageDirectory::OnMenuView(wxCommandEvent &)
{
    Packer::ArchiveWD arch(GetPath());

    NotebookLeft::self->AddPage(new PageInfoWD(GetPath()), GetPath());
}
