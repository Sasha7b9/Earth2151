// 2022/10/03 09:54:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
* �� ���� �������� ������������ ���������� ������ � ����������� ���������
*/

class PageTree : public wxTreeCtrl
{
public:
    PageTree(const wxString &file_path);
private:

    void OnMouseRightUpEvent(wxMouseEvent &);
    void OnMenuClose(wxCommandEvent &);
};
