// 2025/03/25 20:55:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Scripting/Objects/Object.h"
#include "Game/Scripting/Objects/Campaign.h"
#include "Game/Scripting/Objects/Mission.h"


class ScriptEngine : public Global<ScriptEngine>
{
public:

    ScriptEngine();

    virtual ~ScriptEngine();
};


extern ScriptEngine *TheScriptEngine;

extern Campaign *TheCampaign;

extern Mission *TheMission;
