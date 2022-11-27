// 2022/11/26 21:30:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Textures/Image.h"


Image::Image(int width, int height) : wxImage(width, height)
{
}


void Image::SetPixel(int x, int y, uint8 r, uint8 g, uint8 b, uint8)
{
    SetRGB(x, y, r, g, b);
}


void Image::SaveAsBMP(string dir, string name, int i) const
{
    if (!wxDir::Exists(dir))
    {
        wxDir::Make(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    wxString out_file = wxString::Format("%s\\%s_%d_%dx%d.png", dir.c_str(), name, i, GetWidth(), GetHeight());

    SaveFile(out_file);
}
