// 2022/09/25 14:30:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook/Notebook.h"


class NotebookLeft : public Notebook
{
public:
    NotebookLeft(wxWindow *);

    static NotebookLeft *self;

    void DeletePageRaw(wxWindow *);
};
