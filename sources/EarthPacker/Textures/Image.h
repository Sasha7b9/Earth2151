// 2022/11/26 21:30:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Image : public wxImage
{
public:
    Image(int w, int h);
    void SetPixel(int x, int y, uint8 r, uint8 g, uint8 b, uint8 a);
    void SaveAsBMP(string dir, string name, int i) const;
};
