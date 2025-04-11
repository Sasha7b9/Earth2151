// 2025/03/25 21:17:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObject.h"


// Хранит все скрипты из каталога Earth2150/Scripts

class ScriptStorage : public Global<ScriptStorage>
{
public:
    ScriptStorage();
    ~ScriptStorage();

    S::SObject *GetObject(pchar file_name);

    Map<S::SObject> objects;
};


extern ScriptStorage *TheScriptStorage;
