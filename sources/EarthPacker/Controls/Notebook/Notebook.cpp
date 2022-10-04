// 2022/09/25 14:31:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook/Notebook.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY)
{

}


void Notebook::SetSizeRaw(const wxSize &size)
{
    int numpages = GetPageCount();

    for (int i = 0; i < numpages; i++)
    {
        GetPage(i)->SetMinSize(size);
        GetPage(i)->SetMaxSize(size);
        GetPage(i)->SetSize(size);
        GetPage(i)->Refresh();
    }
}


bool Notebook::DeletePage(size_t n)
{
    if ((GetPageCount() - 2) < n)
    {
        return wxNotebook::DeletePage(n);
    }

    return false;
}
