// 2022/10/07 09:33:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Canvas.h"


class PageInfo;


class Description
{
public:
    virtual ~Description() {}
    virtual int Size() const = 0;
    virtual void DrawLine(const PageInfo *, int y, int num_line) const = 0;
protected:
    int DrawCell(const PageInfo *, int x, int y, int width, const wxString &) const;
    int DrawCell(const PageInfo *, int x, int y, int width, int value) const;
};
