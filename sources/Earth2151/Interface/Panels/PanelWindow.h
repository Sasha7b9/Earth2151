// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Controls/Canvas.h"


class PanelWindow : public Canvas
{
public:
    PanelWindow(const Vector2D &);
    virtual ~PanelWindow();

    void SetMovementMutator(const Point2D &posShow, const Point2D &posHide, const Vector2D &speed);

    void Toggle();

    virtual void PreprocessWidget() override;

    void Clear();

    // Если true, то мышь находится над панелью
    bool UnderMouse() const;

protected:
    virtual void UpdateWidget() override {};
};
