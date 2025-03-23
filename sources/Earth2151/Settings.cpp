// 2025/03/22 17:19:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Settings.h"
#include "Utils/Local.h"


ApplicationMode::E ApplicationMode::current = ApplicationMode::Game;
Language::E Language::current = Language::English;
TypeGame::E TypeGame::current = TypeGame::EftBP;


void ApplicationMode::Set(ApplicationMode::E mode)
{
    current = mode;
}


void TypeGame::Set(TypeGame::E type)
{
    current = type;
}


void Language::Set(Language::E lang)
{
    current = lang;

    Local::Init();
}
