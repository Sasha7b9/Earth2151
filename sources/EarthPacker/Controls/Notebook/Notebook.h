// 2022/09/25 14:31:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Notebook : public wxNotebook
{
public:
    Notebook(wxWindow *);

    void SetSizeRaw(const wxSize &);

    virtual bool DeletePage(size_t n) override;

private:
};
