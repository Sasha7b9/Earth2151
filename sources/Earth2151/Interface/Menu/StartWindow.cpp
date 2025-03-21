// 2024/09/20 09:47:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"


StartWindow *TheStartWindow = nullptr;

#define BTN_CAMPAIGN_ED     FindWidget("btnED")
#define BTN_CAMPAIGN_UCS    FindWidget("btnUCS")
#define BTN_CAMPAIGN_LC     FindWidget("btnLC")
#define BTN_NETWORK         FindWidget("btnNetwork")
#define BTN_SKIRMISH        FindWidget("btnSkirmish")
#define BTN_EDITOR          FindWidget("btnEditor")
#define BTN_ABOUT           FindWidget("btnAbout")
#define BTN_QUIT            FindWidget("btnQuit")


StartWindow::StartWindow() :
    Window("panels/Start"),
    Global<StartWindow>(TheStartWindow)
{

}


void StartWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_CAMPAIGN_ED->SetObserver(&buttonEDObserver);
    BTN_CAMPAIGN_UCS->SetObserver(&buttonUCSObserver);
    BTN_CAMPAIGN_LC->SetObserver(&buttonLCObserver);
    BTN_NETWORK->SetObserver(&buttonNetworkObserver);
    BTN_SKIRMISH->SetObserver(&buttonSkirmishObserver);
    BTN_EDITOR->SetObserver(&buttonEditorObserver);
    BTN_ABOUT->SetObserver(&buttonAboutObserver);
    BTN_QUIT->SetObserver(&buttonQuitObserver);

    BTN_CAMPAIGN_UCS->HideWidget();
    BTN_CAMPAIGN_LC->HideWidget();
    BTN_NETWORK->HideWidget();
    BTN_SKIRMISH->HideWidget();
    BTN_ABOUT->HideWidget();
}

void StartWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_CAMPAIGN_ED)
        {
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_CAMPAIGN_UCS)
        {

        }
        else if (widget == BTN_CAMPAIGN_LC)
        {

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
