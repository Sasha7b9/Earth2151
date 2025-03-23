// 2025/03/17 21:12:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/EnterGameWindow.h"


ChoicePlayerWindow *TheChoicePlayerWindow = nullptr;


#define BTN_APPLY  ((PushButtonWidget *)FindWidget("btnApply"))
#define BTN_DELETE ((PushButtonWidget *)FindWidget("btnDelete"))
#define BTN_BACK   ((PushButtonWidget *)FindWidget("btnBack"))


ChoicePlayerWindow::ChoicePlayerWindow() :
    Window("panels/ChoicePlayer"),
    Global<ChoicePlayerWindow>(TheChoicePlayerWindow)
{

}


void ChoicePlayerWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_APPLY->SetObserver(&btnObserver);
    BTN_DELETE->SetObserver(&btnObserver);
    BTN_BACK->SetObserver(&btnObserver);

    BTN_DELETE->HideWidget();

    listPlayers = (ListWidget *)FindWidget("listPlayers");
    listPlayers->SetItemSpacing(20);
}


void ChoicePlayerWindow::Localize()
{

}


void ChoicePlayerWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_APPLY)
        { 
            TheInterfaceMgr->RemoveWidget(TheChoicePlayerWindow);
            TheInterfaceMgr->AddWidget(TheEnterGameWindow);
        }
        else if (widget == BTN_DELETE)
        {

        }
        else if (widget == BTN_BACK)
        {
            TheInterfaceMgr->RemoveWidget(TheChoicePlayerWindow);
            TheInterfaceMgr->AddWidget(TheStartWindow);
        }
    }
}


#undef BTN_BACK
