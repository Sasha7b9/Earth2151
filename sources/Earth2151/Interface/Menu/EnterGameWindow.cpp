// 2025/03/17 22:44:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/EnterGameWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Earth2151.h"


EnterGameWindow *TheEnterGameWindow = nullptr;


#define BTN_NEW FindWidget("btnNewGame")
#define BTN_LOAD FindWidget("btnLoadGame")
#define BTN_TUTORIAL FindWidget("btnTutorial")
#define BTN_VIDEO FindWidget("btnVideo")
#define BTN_BACK FindWidget("btnBack")
#define BTN_QUIT FindWidget("btnQuit")


EnterGameWindow::EnterGameWindow() : Window("panels/EnterGame"), Global<EnterGameWindow>(TheEnterGameWindow)
{
}


void EnterGameWindow::PreprocessWidget()
{
    Pi::Window::PreprocessWidget();

    BTN_NEW->SetObserver(&btnNewGameObserver);
    BTN_LOAD->SetObserver(&btnLoadGameObserver);
    BTN_TUTORIAL->SetObserver(&btnTutorialObserver);
    BTN_VIDEO->SetObserver(&btnVideoObserver);
    BTN_BACK->SetObserver(&btnBackObserver);
    BTN_QUIT->SetObserver(&btnOuitObserver);

    BTN_NEW->HideWidget();
    BTN_LOAD->HideWidget();
    BTN_VIDEO->HideWidget();
}


void EnterGameWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_NEW)
        {

        }
        else if (widget == BTN_LOAD)
        {

        }
        else if (widget == BTN_TUTORIAL)
        {
            TheInterfaceMgr->RemoveWidget(TheEnterGameWindow);
            TheEarth2151->LoadLevel();
        }
        else if (widget == BTN_VIDEO)
        {

        }
        else if (widget == BTN_BACK)
        {
            TheInterfaceMgr->RemoveWidget(TheEnterGameWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_QUIT)
        {
            ConfigDataDescription::WriteEngineConfig(TheEngine->GetConfigFileName());
            TheEngine->Quit();
        }
    }
}
