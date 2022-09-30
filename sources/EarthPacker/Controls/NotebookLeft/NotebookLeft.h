// 2022/09/25 14:30:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"
#include "Controls/NotebookLeft/PageDirectory.h"


class NotebookLeft : public Notebook
{
public:
    NotebookLeft(wxWindow *);

private:
    PageDirectory *pageDirectory = nullptr;

    void OnFileActivated(wxTreeEvent &);
    void OnFileSelected(wxTreeEvent &);
};
