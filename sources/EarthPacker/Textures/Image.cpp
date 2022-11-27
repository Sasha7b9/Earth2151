// 2022/11/26 21:30:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Textures/Image.h"


Image::Image(int width, int height)
{
    image.Resize({ width, height }, {0, 0});
}


void Image::SetPixel(int x, int y, uint8 r, uint8 g, uint8 b, uint8)
{
    image.SetRGB(x, y, r, g, b);
}


void Image::SaveAsBMP(string dir, string name, int i) const
{
    if (!wxDir::Exists(dir))
    {
        wxDir::Make(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    wxString out_file = wxString::Format("%s%d%dx%d.png", name, i, image.GetWidth(), image.GetHeight());

    image.SaveFile(out_file);
}
