// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Panels/PanelWindow.h"
#include "Interface/Panels/PanelMap.h"
#include "Interface/Panels/PanelMain.h"
#include "Earth2151.h"


class TButton;
class PanelMain;


class PanelBottom : public PanelWindow, public Global<PanelBottom>
{
public:
    PanelBottom();
    ~PanelBottom()
    {
        delete ThePanelMain;
        delete ThePanelMap;
    }

private:
    TButton *btnHideMap = nullptr;
    TButton *btnHideMain = nullptr;
    TButton *btnMenu = nullptr;
    TButton *btnLoad = nullptr;

    WidgetObserver<PanelMain> observerPanelMainHide;
    WidgetObserver<PanelMap> observerPanelMapHide;
    WidgetObserver<PanelBottom> observerButtonload{ this, &PanelBottom::HanbleButtonLoadEvent };

    void HanbleButtonLoadEvent(Widget *button, const WidgetEventData *data);
};


extern PanelBottom *ThePanelBottom;
