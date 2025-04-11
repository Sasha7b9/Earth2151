// 2025/03/17 21:12:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/EnterGameWindow.h"
#include "Interface/Menu/ConfirmRemovePlayerWindow.h"
#include "Utils/Locale.h"
#include "Utils/Users.h"


ChoicePlayerWindow *TheChoicePlayerWindow = nullptr;


#define BTN_SELECT_PLAYER ((PushButtonWidget *)FindWidget("btnSelectPlayer"))
#define BTN_DELETE        ((PushButtonWidget *)FindWidget("btnDelete"))
#define BTN_RETURN        ((PushButtonWidget *)FindWidget("btnReturn"))
#define TXT_LIST_PLAYERS  ((TextWidget *)FindWidget("txtListPlayers"))
#define TXT_NEW_PLAYER    ((TextWidget *)FindWidget("txtNewPlayer"))
#define EDIT_NEW_PLAYER   ((EditTextWidget *)FindWidget("lineNewPlayer"))
#define LIST_PLAYERS      ((ListWidget *)FindWidget("listPlayers"))


ChoicePlayerWindow::ChoicePlayerWindow() :
    Window("panels/ChoicePlayer"),
    Global<ChoicePlayerWindow>(TheChoicePlayerWindow)
{
}


void ChoicePlayerWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    BTN_SELECT_PLAYER->SetObserver(&widgetObserver);
    BTN_DELETE->SetObserver(&widgetObserver);
    BTN_RETURN->SetObserver(&widgetObserver);
    EDIT_NEW_PLAYER->SetObserver(&widgetObserver);
    LIST_PLAYERS->SetObserver(&widgetObserver);

    EDIT_NEW_PLAYER->SetText("");

    FillListUsers();

    SetActivationKeyWidget(kActivationKeyEnter, EDIT_NEW_PLAYER);

    Localize();
}


void ChoicePlayerWindow::Localize()
{
    if (!TheChoicePlayerWindow)
    {
        return;
    }

    Window::PreprocessWidget();

    TXT_LIST_PLAYERS->SetText(_L("trListPlayers"));
    TXT_NEW_PLAYER->SetText(_L("trNewPlayer"));
    BTN_DELETE->SetText(_L("trRemove"));
    BTN_SELECT_PLAYER->SetText(_L("trSelectPlayer"));
    BTN_RETURN->SetText(_L("trReturn"));
}


void ChoicePlayerWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_SELECT_PLAYER)
        {
            StartEnterGameWindowIfNeed();
        }
        else if (widget == BTN_DELETE)
        {
            TheInterfaceMgr->RemoveWidget(TheChoicePlayerWindow);
            TheInterfaceMgr->AddWidget(TheConfirmDeletePlayerWindow);
        }
        else if (widget == BTN_RETURN)
        {
            TheInterfaceMgr->RemoveWidget(TheChoicePlayerWindow);
            TheInterfaceMgr->AddWidget(TheStartWindow);
        }
        else if (widget == EDIT_NEW_PLAYER)
        {
            // Сюда попадаем по нажатию кнопки Enter

            StartEnterGameWindowIfNeed();
        }
        else if (widget == LIST_PLAYERS)
        {
            EDIT_NEW_PLAYER->SetText("");
            StartEnterGameWindowIfNeed();
        }
    }
    else if (eventData->eventType == kEventWidgetChange)
    {
        if (widget == EDIT_NEW_PLAYER)
        {
            // Сюда попадаем по нажатию символов
        }
    }
}


void ChoicePlayerWindow::StartEnterGameWindowIfNeed()
{
    String<> name_player = ResolveNamePlayer();

    if (name_player.GetStringLength())
    {
        TheEnterGameWindow->SetNamePlayer(name_player);
        TheInterfaceMgr->RemoveWidget(TheChoicePlayerWindow);
        TheInterfaceMgr->AddWidget(TheEnterGameWindow);
    }
}


String<> ChoicePlayerWindow::ResolveNamePlayer()
{
    String<> name_player = EDIT_NEW_PLAYER->GetText();

    if (!name_player.GetStringLength())
    {
        TextWidget *text = (TextWidget *)LIST_PLAYERS->GetFirstSelectedListItem();

        if (text)
        {
            name_player = text->GetText();
        }
    }

    if (name_player.GetStringLength())
    {
        if (!Users::Exist(name_player))
        {
            Users::Create(name_player);
        }

        Users::Enter(name_player);
    }

    return name_player;
}


void ChoicePlayerWindow::FillListUsers()
{
    LIST_PLAYERS->PurgeListItems();

    String<> name = Users::GetFirst();

    while (name.GetStringLength())
    {
        LIST_PLAYERS->AppendListItem(name);
        name = Users::GetNext();
    }

    listPlayers = (ListWidget *)FindWidget("listPlayers");
    listPlayers->SetItemSpacing(17);

}

#undef BTN_BACK
