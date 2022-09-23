// 2022/09/23 13:48:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ControlDir : public wxGenericDirCtrl
{
public:

    ControlDir(wxWindow *);

    static ControlDir *self;

private:
};
