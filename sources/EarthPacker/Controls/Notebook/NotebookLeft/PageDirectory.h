// 2022/09/23 13:48:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PageDirectory : public wxGenericDirCtrl
{
public:

    PageDirectory(wxWindow *);

    static PageDirectory *self;

private:

    void OnFileActivated(wxTreeEvent &);
    void OnFileSelected(wxTreeEvent &);
    void OnRightClick(wxTreeEvent &);

    void OnMenuUnpack(wxCommandEvent &);
};
