// 2025/03/24 22:11:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SObjectX.h"


void S::ObjectX::SendCustomEvent(int param1, int param2, int param3, int param4, int param5)
{

}


void S::ObjectX::SetAvailableWorlds(int worlds_mask)
{
    LOG_WRITE("\"%s\" %d", __FUNCTION__, worlds_mask);
}


void S::ObjectX::ShowVideo(pchar video_name)
{

}


void S::ObjectX::EndGame(pchar outro)
{

}


bool S::ObjectX::ExecuteOperatorCall(OperatorCall *oper)
{
    if (S::SObject::ExecuteOperatorCall(oper))
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
