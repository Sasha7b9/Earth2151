// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Controls/Canvas.h"


class ObjectViewportWidget : public Canvas
{

public:

    ObjectViewportWidget(const Vector2D &size, float focalLength);

private:

    WorldViewportWidget *viewport = nullptr;
};
