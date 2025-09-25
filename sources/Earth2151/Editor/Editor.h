// 2025/03/19 21:00:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Earth2150Types.h"
#include "Game/Parameters/Parameters2150.h"
#include "GameWorld.h"


class Editor : public Global<Editor>
{
public:

    Editor();

    virtual ~Editor()
    {
        UnloadLevel();
    }

    void OpenWindowLoadLevel();

    void LoadLevel(pchar);

    void UnloadLevel();

private:

    static void EscapeCallback(void *);

    void CreateLandscape(GameWorld *);

    void CreateGameObjects();

    Level2150 *level = nullptr;
};


extern Editor *TheEditor;
