// 2025/03/19 21:00:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Earth2150Types.h"
#include "Earth2150/Parameters/Parameters2150.h"


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

    void CreateLandscape();

    void CreateGameObjects();
};


extern Editor *TheEditor;
