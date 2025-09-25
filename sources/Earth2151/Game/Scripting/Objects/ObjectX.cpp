// 2025/03/24 22:11:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Objects/Object.h"


void ObjectX::SendCustomEvent(int param1, int param2, int param3, int param4, int param5)
{
    LOG_WARNING_HI("Not implemented");
}


void ObjectX::SetAvailableWorlds(int worlds_mask)
{
    LOG_WARNING_HI("Not implemented");
}


void ObjectX::ShowVideo(pchar video_name)
{
    LOG_WARNING_HI("Not implemented");
}


void ObjectX::EndGame(pchar outro)
{
    LOG_WARNING_HI("Not implemented");
}


void ObjectX::SetTime(int nDayTick)
{
    LOG_WARNING_HI("Not implemented");
}


bool ObjectX::ExecuteOperatorFunction(OperatorFunction *oper)
{
    if (SObject::ExecuteOperatorFunction(oper))
    {
        return true;
    }

    if (oper->name_func == "SetAvailableWorlds")
    {
        SetAvailableWorlds(
            oper->ResolveParameterInt(0));

        return true;
    }

    return false;
}
