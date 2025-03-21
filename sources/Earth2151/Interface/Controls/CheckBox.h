// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Controls/Canvas.h"


class CheckBoxSkin : public Canvas
{
public:
    enum
    {
        StateUnchecked,
        StateChecked,
        NnumSkins
    };

    CheckBoxSkin(pchar, uint state, bool shadowed, bool checked);
    ~CheckBoxSkin();

private:
    void DrawSkin(const ColorRGBA &coorBackground, const ColorRGBA &colorBorder, const ColorRGBA &colorFill, const ColorRGBA &colorCheck, uint state, pchar);
};

enum TypeCheckBox
{
    kTypeNormal
};


class CheckBox : public Widget
{
public:
    CheckBox(TypeCheckBox type, const Vector2D &position, pchar);
    ~CheckBox();

private:
    void SetState(uint state);

    uint type = 0;
#define NUM_SHADOWS 2
#define NUM_CHECKED 2
    CheckBoxSkin *skins[CheckBoxSkin::NnumSkins][NUM_SHADOWS][NUM_CHECKED]; // [numskin][shadowed][checked]
    uint state = CheckBoxSkin::StateUnchecked;
    bool shadowed = false;
    bool checked = false;
};
