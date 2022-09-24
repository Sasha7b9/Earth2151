// 2022/09/24 10:26:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class FileInputStream : public wxFileInputStream
{
public:
    FileInputStream(const wxString &file_path);

    wxMemoryBuffer ReadAll();
    wxMemoryBuffer ReadBytes(int offset, int num_bytes);
    uint ReadUINT(int offset);
private:
};
