// 2022/05/05 09:50:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


namespace Font
{
    void Set(wxFont);

    wxFont Get();

    void SetSize(int);
}
