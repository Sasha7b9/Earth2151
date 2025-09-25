// 2025/03/17 22:44:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/EnterGameWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Earth2151.h"
#include "Utils/Locale.h"
#include "Game/Game.h"


EnterGameWindow *TheEnterGameWindow = nullptr;


#define BTN_NEW      ((PushButtonWidget *)FindWidget("btnNewGame"))
#define BTN_LOAD     ((PushButtonWidget *)FindWidget("btnLoadGame"))
#define BTN_TUTORIAL ((PushButtonWidget *)FindWidget("btnTutorial"))
#define BTN_VIDEO    ((PushButtonWidget *)FindWidget("btnVideo"))
#define BTN_BACK     ((PushButtonWidget *)FindWidget("btnBack"))
#define BTN_QUIT     ((PushButtonWidget *)FindWidget("btnQuit"))


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

    BTN_LOAD->HideWidget();
    BTN_VIDEO->HideWidget();

    Localize();
}


void EnterGameWindow::Localize()
{
    if (!TheEnterGameWindow)
    {
        return;
    }

    Window::PreprocessWidget();

    BTN_NEW->SetText(_L("trNewGame"));
    BTN_LOAD->SetText(_L("trLoadGame"));
    BTN_TUTORIAL->SetText(_L("trTutorial"));
    BTN_VIDEO->SetText(_L("trVideos"));
    BTN_BACK->SetText(_L("trReturn"));
    BTN_QUIT->SetText(_L("trQuit"));
}


void EnterGameWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_NEW)
        {
            TypeGame::Set(TypeGame::Campaign);
            TheInterfaceMgr->RemoveWidget(TheEnterGameWindow);
            ApplicationMode::Set(ApplicationMode::Game);
            new Game();
        }
        else if (widget == BTN_LOAD)
        {

        }
        else if (widget == BTN_TUTORIAL)
        {
            TypeGame::Set(TypeGame::Tutorial);
            TheInterfaceMgr->RemoveWidget(TheEnterGameWindow);
            ApplicationMode::Set(ApplicationMode::Game);
            new Game();
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


void EnterGameWindow::SetNamePlayer(const String<> &)
{

}


#undef BTN_QUIT
#undef BTN_LOAD
