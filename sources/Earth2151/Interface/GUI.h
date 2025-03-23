// 2025/02/14 22:34:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2151.h"


class GUI : public Widget, public Global<GUI>
{
public:

    GUI();
    ~GUI();

    void Hide();
    void Show();

    void MouseLeftIsPress();
    void MouseLeftIsRelease();

    bool UnderMouse() const;

    // Если не nullptr, то этот виджет захвачен мышкой для перемещения
    static Widget *trackedWidget;

    // Позиция курсора при захвате виджета
    static Point2D mouse_pos;

    // Позиция виджета при захывате виджета
    static Point2D widget_pos;
};


extern GUI *TheGUI;
