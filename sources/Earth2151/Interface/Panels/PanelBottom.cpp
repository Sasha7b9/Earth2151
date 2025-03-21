// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Panels/PanelBottom.h"
#include "Settings.h"
#include "Interface/Controls/Button.h"
#include "Interface/Panels/PanelMap.h"
#include "Interface/Panels/PanelMain.h"
//#include "Interface/Menu/MenuGame.h"


PanelBottom *ThePanelBottom = nullptr;


PanelBottom::PanelBottom()
    : PanelWindow(SET::GUI::BOTTOM::SIZE()),
    Global<PanelBottom>(ThePanelBottom)
{
    SetWidgetPosition({ (float)SET::GUI::BOTTOM::X(), (float)SET::GUI::BOTTOM::Y() });

    btnHideMap = new TButton(TButton::Type::Normal, {10.0f, 2.0f}, "MAP");
    AppendNewSubnode(btnHideMap);

    btnHideMain = new TButton(TButton::Type::Normal, {100.0f, 2.0f}, "MAIN");
    AppendNewSubnode(btnHideMain);

    btnMenu = new TButton(TButton::Type::Normal, {SET::WINDOW::SIZE().x - 300.0f, 2.0f}, "MENU");
    AppendNewSubnode(btnMenu);

    btnHideMain->SetObserver(&observerPanelMainHide);
    btnHideMap->SetObserver(&observerPanelMapHide);
//    btnMenu->SetObserver(&observerButtonMenu);
}
