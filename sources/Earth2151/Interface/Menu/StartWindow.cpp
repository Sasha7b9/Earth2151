// 2024/09/20 09:47:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Interface/Menu/StartSettingsWindow.h"
#include "Utils/Locale.h"
#include "Editor/Editor.h"
#include "Settings.h"


StartWindow *TheStartWindow = nullptr;


#define BTN_CAMPAIGN_ED     ((PushButtonWidget *)FindWidget("btnED"))               // Евроальянс
#define BTN_CAMPAIGN_UCS    ((PushButtonWidget *)FindWidget("btnUCS"))              // Атлантический союз
#define BTN_CAMPAIGN_LC     ((PushButtonWidget *)FindWidget("btnLC"))               // Дети Селены
#define BTN_SETTINGS        ((PushButtonWidget *)FindWidget("btnSettings"))         // Настройки
#define BTN_NETWORK         ((PushButtonWidget *)FindWidget("btnNetwork"))
#define BTN_SKIRMISH        ((PushButtonWidget *)FindWidget("btnSkirmish"))
#define BTN_VIEWER          ((PushButtonWidget *)FindWidget("btnEditor"))           // Просмотрщик
#define BTN_ABOUT           ((PushButtonWidget *)FindWidget("btnAbout"))
#define BTN_QUIT            ((PushButtonWidget *)FindWidget("btnQuit"))             // Выход
#define TXT_HINT            ((TextWidget *)FindWidget("txtHint"))


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
    BTN_VIEWER->SetObserver(&buttonObserver);
    BTN_ABOUT->SetObserver(&buttonObserver);
    BTN_QUIT->SetObserver(&buttonObserver);

    Localize();

    BTN_NETWORK->HideWidget();
    BTN_SKIRMISH->HideWidget();
    BTN_ABOUT->HideWidget();

    if (NameGame().IsEftBP())
    {
        BTN_CAMPAIGN_LC->ShowWidget();
    }
    else
    {
        BTN_CAMPAIGN_LC->HideWidget();
    }
}


void StartWindow::Localize()
{
    if (!TheStartWindow)
    {
        return;
    }

    ((TextWidget *)FindWidget("txtTitle"))->SetText(_L("trEarth 2151"));

    BTN_CAMPAIGN_ED->SetText(_L("trED"));
    BTN_CAMPAIGN_UCS->SetText(_L("trUCS"));
    BTN_CAMPAIGN_LC->SetText(_L("trLC"));
    BTN_SETTINGS->SetText(_L("trSettings"));
    BTN_NETWORK->SetText(_L("trNetworkGame"));
    BTN_SKIRMISH->SetText(_L("trSingleMission"));
    BTN_VIEWER->SetText(_L("trEditor"));
    BTN_ABOUT->SetText(_L("trAutors"));
    BTN_QUIT->SetText(_L("trExit"));

    static const pchar hints[NameGame::Count] =
    {
        "trEftBP",
        "trTMP",
        "trLS"
    };

    TXT_HINT->SetText(_L(hints[NameGame().Current()]));
}


void StartWindow::HandleButtonEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_CAMPAIGN_ED)
        {
            TypeCampaign::Set(TypeCampaign::ED);
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_CAMPAIGN_UCS)
        {
            TypeCampaign::Set(TypeCampaign::UCS);
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_CAMPAIGN_LC)
        {
            TypeCampaign::Set(TypeCampaign::LC);
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            TheInterfaceMgr->AddWidget(TheChoicePlayerWindow);
        }
        else if (widget == BTN_SETTINGS)
        {
            TheInterfaceMgr->RemoveWidget(TheStartWindow);
            TheInterfaceMgr->AddWidget(TheStartSettingsWindow);
        }
        else if (widget == BTN_NETWORK)
        {

        }
        else if (widget == BTN_SKIRMISH)
        {

        }
        else if (widget == BTN_VIEWER)
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
