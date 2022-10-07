// 2022/09/23 13:48:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/PageDirectory.h"
#include "Controls/Notebook/NotebookRight/PageInfo.h"
#include "Resources/Archive.h"
#include "Controls/Notebook/NotebookLeft/NotebookLeft.h"
#include "Controls/Notebook/NotebookLeft/PageTree.h"
#include "Models/Model.h"


PageDirectory *PageDirectory::self = nullptr;


enum
{
    ID_POPUP_UNPACK = wxID_HIGHEST + 1,
    ID_POPUP_VIEW,
    ID_POPUP_VIEW_MSH
};


PageDirectory::PageDirectory(wxWindow *parent) : wxGenericDirCtrl(parent)
{
    self = this;

    wxWindowBase::SetMinClientSize({ 300, 100 });
    SetClientSize({ 300, 100 });

    wxGenericDirCtrl::ExpandPath("C:/Temp/Earth2150");

    Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &PageDirectory::OnFileSelected, this);
    Bind(wxEVT_DIRCTRL_FILEACTIVATED, &PageDirectory::OnFileActivated, this);
    Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &PageDirectory::OnRightClick, this);
}

void PageDirectory::OnFileSelected(wxTreeEvent &event) //-V2009
{
    wxString path = GetPath(event.GetItem());

    wxFileName file_name(path);

    if (file_name.GetExt() == "wd")
    {
        DescriptionArchive description;

        Archive arch(path);

        arch.GetDescription(description);

        PageInfo::self->SetDescriptionFile(description);
    }
    else if(file_name.GetExt() == "msh")
    {
        Model model(file_name);

        file_name.SetExt("xml");
    }
}


void PageDirectory::OnFileActivated(wxTreeEvent &) //-V2009
{

}


void PageDirectory::OnRightClick(wxTreeEvent &event)
{
    SelectPath(GetPath(event.GetItem()));

    wxFileName file_name(GetPath(event.GetItem()));

    wxMenu menu;

    if (file_name.GetExt() == "wd")
    {
        menu.Append(ID_POPUP_UNPACK, "Unpack");
        Bind(wxEVT_MENU, &PageDirectory::OnMenuUnpack, this, ID_POPUP_UNPACK);
    }
    else if (file_name.GetExt() == "msh")
    {
        menu.Append(ID_POPUP_VIEW_MSH, "View");
        Bind(wxEVT_MENU, &PageDirectory::OnMenuViewMSH, this, ID_POPUP_VIEW_MSH);
    }

    PopupMenu(&menu, event.GetPoint());

    event.Skip();

//    menu.Append(ID_POPUP_VIEW, "View");
//    Bind(wxEVT_MENU, &PageDirectory::OnMenuView, this, ID_POPUP_VIEW);
}


void PageDirectory::OnMenuUnpack(wxCommandEvent &)
{
    wxFileName file_name(GetPath());

    if (file_name.GetExt() == "wd")
    {
        Archive arch(file_name);
        arch.ReadContent();
        arch.Unpack(file_name.GetPath() + wxFileName::GetPathSeparator());

        wxGenericDirCtrl::CollapseTree();
        wxGenericDirCtrl::ExpandPath("C:/Temp/Earth2150");
    }
}


void PageDirectory::OnMenuView(wxCommandEvent &)
{
    Archive arch(GetPath());

    NotebookLeft::self->DeletePage(1);

    NotebookLeft::self->AddPage(new PageTree(GetPath()), GetPath());

    NotebookLeft::self->ChangeSelection(1);
}


void PageDirectory::OnMenuViewMSH(wxCommandEvent &)
{

}
