// 2022/09/24 10:28:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/FileInputStream.h"


FileInputStream::FileInputStream(const wxString &file_path) : wxFileInputStream(file_path)
{

}
