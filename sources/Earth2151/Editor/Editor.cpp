// 2025/03/20 15:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Editor/Editor.h"


Editor *TheEditor = nullptr;


Editor::Editor() : Global<Editor>(TheEditor)
{

}

