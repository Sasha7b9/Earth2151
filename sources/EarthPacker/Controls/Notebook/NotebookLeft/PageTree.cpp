// 2022/10/03 09:54:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/NotebookLeft/PageTree.h"
#include "Controls/Notebook/NotebookLeft/NotebookLeft.h"


PageTree::PageTree(const wxString &file) : wxTreeCtrl(NotebookLeft::self)
{
    Bind(wxEVT_RIGHT_UP, &PageTree::OnMouseRightUpEvent, this);

    wxTreeItemId root = AddRoot(file);
    wxArrayTreeItemIds items;
    items.Add(AppendItem(root, "Item 1"));
    items.Add(AppendItem(root, "Item 2"));

    wxTreeItemId root1 = items.Item(0);
    AppendItem(root1, "subitem 1");

    Expand(root);
}


void PageTree::OnMouseRightUpEvent(wxMouseEvent &event)
{
    wxMenu menu;

    menu.Append(wxID_EXIT, "Close");
    Bind(wxEVT_MENU, &PageTree::OnMenuClose, this, wxID_EXIT);

    PopupMenu(&menu, event.GetPosition());

    event.Skip();
}


void PageTree::OnMenuClose(wxCommandEvent &)
{
    NotebookLeft::self->DeletePageRaw(this);
}
