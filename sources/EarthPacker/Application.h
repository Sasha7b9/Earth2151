// 2022/04/29 13:56:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)

class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;

    void OnTimer(wxTimerEvent &);

private:

    wxTimer timer;

    void Init();

    void Update();

    void OnClose();
};
