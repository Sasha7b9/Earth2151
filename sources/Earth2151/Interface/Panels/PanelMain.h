// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Panels/PanelWindow.h"


class PanelMain : public PanelWindow, public Global<PanelMain>
{
public:

    PanelMain();

    virtual ~PanelMain();

    void HandleHideShow(Widget *, const WidgetEventData *);
};


extern PanelMain *ThePanelMain;
