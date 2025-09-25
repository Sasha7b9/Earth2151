// 2025/03/25 21:17:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Scripting/Objects/Object.h"


// Хранит все скрипты из каталога Earth2150/Scripts

class ScriptStorage : public Global<ScriptStorage>
{
public:
    ScriptStorage();
    ~ScriptStorage();

    SObject *GetObject(pchar file_name);

    Map<SObject> objects;
};


extern ScriptStorage *TheScriptStorage;
