// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Panels/PanelWindow.h"
#include "Interface/Panels/PanelMap.h"
//#include "Interface/Menu/MenuGame.h"
#include "Interface/Panels/PanelMain.h"


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

    WidgetObserver<PanelMain> observerPanelMainHide{ (new PanelMain(), ThePanelMain), &PanelMain::HandleHideShow };
    WidgetObserver<PanelMap> observerPanelMapHide{ (new PanelMap(), ThePanelMap), &PanelMap::HandleHideShow };
//    WidgetObserver<MenuGame> observerButtonMenu{ (new MenuGame(), MenuGame::self), &MenuGame::HandleOnButtonMenu };
};


extern PanelBottom *ThePanelBottom;
