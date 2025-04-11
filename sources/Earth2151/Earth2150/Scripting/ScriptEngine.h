// 2025/03/25 20:55:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObject.h"
#include "Earth2150/Scripting/Objects/SCampaign.h"


class ScriptEngine : public Global<ScriptEngine>
{
public:

    ScriptEngine();

    virtual ~ScriptEngine();
};


extern ScriptEngine *TheScriptEngine;

extern S::Campaign *TheCampaign;