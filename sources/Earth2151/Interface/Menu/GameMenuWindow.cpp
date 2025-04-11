// 2025/03/18 08:09:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Earth2151.h"
#include "Editor/Editor.h"
#include "Interface/GUI.h"
#include "Cameras.h"
#include "Interface/Menu/StartWindow.h"
#include "Utils/Locale.h"


GameMenuWindow *TheGameMenuWindow = nullptr;


#define BTN_BACK_TO_GAME ((PushButtonWidget *)FindWidget("btnBackToGame"))
#define BTN_EXIT_GAME    ((PushButtonWidget *)FindWidget("btnExitGame"))


GameMenuWindow::GameMenuWindow() : Window("panels/GameMenu"), Global<GameMenuWindow>(TheGameMenuWindow)
{
}


void GameMenuWindow::PreprocessWidget()
{
    Pi::Window::PreprocessWidget();

    BTN_BACK_TO_GAME->SetObserver(&btnBackToGameObserver);
    BTN_EXIT_GAME->SetObserver(&btnExitGameObserver);

    Localize();
}


void GameMenuWindow::Localize()
{
    if (!TheGameMenuWindow)
    {
        return;
    }

    Window::PreprocessWidget();

    BTN_BACK_TO_GAME->SetText(_L("trReturnToGame"));
    BTN_EXIT_GAME->SetText(_L("trFinishGame"));
}


void GameMenuWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_BACK_TO_GAME)
        {
            TheInterfaceMgr->RemoveWidget(TheGameMenuWindow);
            TheEarth2151->Resume();
        }
        else if (widget == BTN_EXIT_GAME)
        {
            TheEarth2151->Resume();
            TheGUI->Hide();
            TheInterfaceMgr->RemoveWidget(TheGameMenuWindow);
            SAFE_DELETE(TheEditor);
            SAFE_DELETE(TheCameraRTS);
            SAFE_DELETE(TheCameraSpectator);
            TheInterfaceMgr->AddWidget(TheStartWindow);
        }
    }
}


bool GameMenuWindow::HandleKeyboardEvent(const KeyboardEventData *eventData)
{
    if (!Pi::Window::HandleKeyboardEvent(eventData))
    {
        if (eventData->eventType == kEventKeyDown)
        {
            if (eventData->keyCode == kKeyCodeEscape)
            {
                TheInterfaceMgr->RemoveWidget(TheGameMenuWindow);

                TheEarth2151->Resume();
            }
        }
    }

    return true;
}
