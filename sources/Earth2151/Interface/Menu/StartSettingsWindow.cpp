// 2025/03/22 16:08:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/StartSettingsWindow.h"
#include "Interface/Menu/StartWindow.h"
#include "Utils/Local.h"
#include "Settings.h"


StartSettingsWindow *TheStartSettingsWindow = nullptr;


#define BTN_CLOSE       ((PushButtonWidget *)FindWidget("btnClose"))
#define MENU_LANG       ((PopupMenuWidget *)FindWidget("pmLanguage"))
#define MENU_GAME       ((PopupMenuWidget *)FindWidget("pmGame"))
#define TXT_SETTINGS    ((TextWidget *)FindWidget("txtSettings"))
#define TXT_GAME        ((TextWidget *)FindWidget("txtGame"))


StartSettingsWindow::StartSettingsWindow() :
    Window("panels/StartSettings"),
    Global<StartSettingsWindow>(TheStartSettingsWindow)
{

}


void StartSettingsWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_CLOSE->SetObserver(&btnObserver);
    MENU_LANG->SetObserver(&btnObserver);
    MENU_GAME->SetObserver(&btnObserver);

    if (MENU_LANG->GetMenuItemCount() == 0)
    {
        MENU_LANG->AppendMenuItem(new MenuItemWidget("English"));
        MENU_LANG->AppendMenuItem(new MenuItemWidget("Polish"));
        MENU_LANG->AppendMenuItem(new MenuItemWidget("Russian"));

        MENU_LANG->SetSelection(0, true);

        MENU_GAME->AppendMenuItem(new MenuItemWidget("Esacape from the Blue Planet"));
        MENU_GAME->AppendMenuItem(new MenuItemWidget("The Moon Project"));
        MENU_GAME->AppendMenuItem(new MenuItemWidget("Lost Souls"));

        MENU_GAME->SetSelection(0, true);
    }

    Localize();
}


void StartSettingsWindow::Localize()
{
    BTN_CLOSE->SetText(_L("trClose"));
    TXT_SETTINGS->SetText(_L("trSettings"));
    TXT_GAME->SetText(_L("trGame"));
}


void StartSettingsWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_CLOSE)
        {
            TheInterfaceMgr->RemoveWidget(this);
            TheInterfaceMgr->AddWidget(TheStartWindow);
        }
    }
    else if (eventData->eventType == kEventWidgetChange)
    {
        if (widget == MENU_LANG)
        {
            Language::Set((Language::E)MENU_LANG->GetSelection());
        }
        else if (widget == MENU_GAME)
        {
            TypeGame::Set((TypeGame::E)MENU_GAME->GetSelection());
        }
    }
}
