// 2022/09/25 14:31:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook/Notebook.h"


class NotebookRight : public Notebook
{
public:
    NotebookRight(wxWindow *);

    static NotebookRight *self;
};
