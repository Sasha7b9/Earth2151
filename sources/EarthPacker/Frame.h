// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/NotebookLeft/PageDirectory.h"
#include "Controls/NotebookLeft/NotebookLeft.h"
#include "Controls/NotebookRight/NotebookRight.h"


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    static Frame *self;

    void OnSize(wxSizeEvent &);

    void OnCloseWindow(wxCloseEvent &);

private:

    wxToolBar     *toolBar = nullptr;
    NotebookLeft  *nbLeft = nullptr;
    NotebookRight *nbRight = nullptr;

    void AddTool(int id, const wxString &label, pchar nameResource, pchar nameResourceDisabled = nullptr);

    void OnClose();
};
