// 2024/09/20 09:47:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Interface/Menu/StartSettingsWindow.h"
#include "Utils/Local.h"
#include "Editor/Editor.h"
#include "Settings.h"


StartWindow *TheStartWindow = nullptr;


#define BTN_CAMPAIGN_ED     ((PushButtonWidget *)FindWidget("btnED"))
#define BTN_CAMPAIGN_UCS    ((PushButtonWidget *)FindWidget("btnUCS"))
#define BTN_CAMPAIGN_LC     ((PushButtonWidget *)FindWidget("btnLC"))
#define BTN_SETTINGS        ((PushButtonWidget *)FindWidget("btnSettings"))
#define BTN_NETWORK         ((PushButtonWidget *)FindWidget("btnNetwork"))
#define BTN_SKIRMISH        ((PushButtonWidget *)FindWidget("btnSkirmish"))
#define BTN_EDITOR          ((PushButtonWidget *)FindWidget("btnEditor"))
#define BTN_ABOUT           ((PushButtonWidget *)FindWidget("btnAbout"))
#define BTN_QUIT            ((PushButtonWidget *)FindWidget("btnQuit"))


StartWindow::StartWindow() :
    Window("panels/Start"),
    Global<StartWindow>(TheStartWindow)
{

}


void StartWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_CAMPAIGN_ED->SetObserver(&buttonObserver);
    BTN_CAMPAIGN_UCS->SetObserver(&buttonObserver);
    BTN_CAMPAIGN_LC->SetObserver(&buttonObserver);
    BTN_SETTINGS->SetObserver(&buttonObserver);
    BTN_NETWORK->SetObserver(&buttonObserver);
    BTN_SKIRMISH->SetObserver(&buttonObserver);
    BTN_EDITOR->SetObserver(&buttonObserver);
    BTN_ABOUT->SetObserver(&buttonObserver);
    BTN_QUIT->SetObserver(&buttonObserver);

    Localize();

    BTN_CAMPAIGN_ED->HideWidget();
    BTN_CAMPAIGN_UCS->HideWidget();
    BTN_CAMPAIGN_LC->HideWidget(); 
    BTN_NETWORK->HideWidget();
    BTN_SKIRMISH->HideWidget();
    BTN_ABOUT->HideWidget();
}


void StartWindow::Localize()
{
    ((TextWidget *)FindWidget("txtEarth2151"))->SetText(_L("trEarth 2151"));

    BTN_CAMPAIGN_ED->SetText(_L("trED"));
    BTN_CAMPAIGN_UCS->SetText(_L("trUCS"));
    BTN_CAMPAIGN_LC->SetText(_L("trLC"));
    BTN_SETTINGS->SetText(_L("trSettings"));
    BTN_NETWORK->SetText(_L("trNetworkGame"));
    BTN_SKIRMISH->SetText(_L("trSingleMission"));
    BTN_EDITOR->SetText(_L("trEditor"));
    BTN_ABOUT->SetText(_L("trAutors"));
    BTN_QUIT->SetText(_L("trExit"));
}


void StartWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_CAMPAIGN_ED)
        {
            TheInterfaceMgr->RemoveWidget(this);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_CAMPAIGN_UCS)
        {

        }
        else if (widget == BTN_CAMPAIGN_LC)
        {

        }
        else if (widget == BTN_SETTINGS)
        {
            TheInterfaceMgr->RemoveWidget(this);
            TheInterfaceMgr->AddWidget(TheStartSettingsWindow);
        }
        else if (widget == BTN_NETWORK)
        {

        }
        else if (widget == BTN_SKIRMISH)
        {

        }
        else if (widget == BTN_EDITOR)
        {
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            ApplicationMode::Set(ApplicationMode::Editor);
            new Editor();
        }
        else if (widget == BTN_ABOUT)
        {

        }
        else if (widget == BTN_QUIT)
        {
            ConfigDataDescription::WriteEngineConfig(TheEngine->GetConfigFileName());
            TheEngine->Quit();
        }
    }
}


#undef BTN_QUIT
