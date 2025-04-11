// 2025/03/20 15:36:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/Menu.h"
#include "Interface/Menu/StartWindow.h"
#include "Interface/Menu/ChoicePlayerWindow.h"
#include "Interface/Menu/GameMenuWindow.h"
#include "Interface/Menu/EnterGameWindow.h"
#include "Interface/Menu/StartSettingsWindow.h"
#include "Interface/Menu/LoadLevelWindow.h"
#include "Interface/Menu/ConfirmRemovePlayerWindow.h"


void EMenu::Init()
{
    new StartWindow();

    new ChoicePlayerWindow();

    new GameMenuWindow();

    new EnterGameWindow();

    new StartSettingsWindow();

    new LoadLevelWindow();

    new ConfirmRemovePlayerWindow();
}


void EMenu::DeInit()
{
    delete TheStartWindow;

    delete TheChoicePlayerWindow;

    delete TheGameMenuWindow;

    delete TheEnterGameWindow;

    delete TheStartSettingsWindow;

    delete TheLoadLevelWindow;

    delete TheConfirmDeletePlayerWindow;
}


void EMenu::Localize()
{
    TheStartWindow->Localize();
    TheEnterGameWindow->Localize();
    TheStartSettingsWindow->Localize();
    TheLoadLevelWindow->Localize();
    TheGameMenuWindow->Localize();
    TheChoicePlayerWindow->Localize();
}
