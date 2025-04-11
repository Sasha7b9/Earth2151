// 2025/04/06 18:51:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/ConfirmRemovePlayerWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Utils/Users.h"
#include "Utils/Locale.h"


ConfirmRemovePlayerWindow *TheConfirmDeletePlayerWindow = nullptr;


#define TXT_REMOVE   ((TextWidget *)FindWidget("txtRemovePlayer"))
#define TXT_MESSAGE  ((TextWidget *)FindWidget("txtMessage"))
#define BTN_REMOVE   ((PushButtonWidget *)FindWidget("btnRemove"))
#define BTN_LEAVE    ((PushButtonWidget *)FindWidget("btnLeave"))


ConfirmRemovePlayerWindow::ConfirmRemovePlayerWindow() :
    Window("panels/ConfirmRemovePlayer"),
    Global<ConfirmRemovePlayerWindow>(TheConfirmDeletePlayerWindow)
{
}


void ConfirmRemovePlayerWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_REMOVE->SetObserver(&widgetObserver);
    BTN_LEAVE->SetObserver(&widgetObserver);

    Localize();
}


void ConfirmRemovePlayerWindow::Localize()
{
    if (!TheConfirmDeletePlayerWindow)
    {
        return;
    }

    Window::PreprocessWidget();

    BTN_REMOVE->SetText(_L("trRemove"));
    BTN_LEAVE->SetText(_L("trLeave"));
    TXT_REMOVE->SetText(_L("trRemovingPlayer"));
    TXT_MESSAGE->SetText(_L("trRequestRemovePlayer") + "\"" + TheChoicePlayerWindow->ResolveNamePlayer() + "\"?");
}


void ConfirmRemovePlayerWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *data)
{
    if (data->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_REMOVE)
        {
            Users::Remove(TheChoicePlayerWindow->ResolveNamePlayer());

            TheInterfaceMgr->RemoveWidget(TheConfirmDeletePlayerWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_LEAVE)
        {
            TheInterfaceMgr->RemoveWidget(TheConfirmDeletePlayerWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
    }
}
