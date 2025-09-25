// 2025/03/25 13:10:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Objects/Mission.h"
#include "Settings.h"


std::vector<Mission *>Mission::missions;


Mission::Mission(pchar f, pchar n, ScriptReader *r) :
    ObjectX(f, n, r)
{
    static int new_id = 0;

    id = new_id;

    new_id++;

    missions.push_back(this);
}


void Mission::Construct(int nMissinNum, pchar strMissionLevel, pchar strMissionScript, pchar strShortBriefing,
    pchar nFlags, int nLongitude, int nLatitude, int nDistanceToBase1, int nDistanceToBase2, int nDistanceToBase3,
    int nNextMission0, int nNextMission1, int nNextMission2, int nNextMission3)
{
    id = nMissinNum;
    file_level = strMissionLevel;
    file_script = strMissionScript;
    briefing = strShortBriefing;
    flags = nFlags;
    longitude = nLongitude;
    latitude = nLatitude;
    d1 = nDistanceToBase1;
    d2 = nDistanceToBase2;
    d3 = nDistanceToBase3;
    id_next1 = nNextMission0;
    id_next2 = nNextMission1;
    id_next3 = nNextMission2;
    id_next4 = nNextMission3;
}


void Mission::Update()
{
    ObjectX::Update();
}


bool Mission::ExecuteOperatorFunction(OperatorFunction *oper)
{
    if (ObjectX::ExecuteOperatorFunction(oper))
    {
        return true;
    }

    if (oper->name_func == "RegisterGoal")
    {
        RegisterGoal(
            oper->ResolveParameterInt(0),
            oper->ResolveParameterStr(1).c_str(),
            oper->ResolveParameterStr(2).c_str(),
            oper->ResolveParameterStr(3).c_str(),
            oper->ResolveParameterStr(4).c_str(),
            oper->ResolveParameterStr(5).c_str(),
            oper->ResolveParameterStr(6).c_str(),
            oper->ResolveParameterStr(7).c_str()
        );

        return true;
    }
    if (oper->name_func == "GetPlayer")
    {

    }

    LOG_ERROR_MID("Not implemented CALL for \"%s\"", oper->name_func.c_str());

    return false;
}


void Mission::CallCamera()
{

}


void Mission::EnableGoal(int num_goal, int enable)
{

}


void Mission::RegisterGoal(int num_goal, pchar goal_text)
{

}


void Mission::SelectUnit(UnitEx unit, int add_to_selection)
{

}


void Mission::SetTimer(int num_timer, int interval)
{

}


void Mission::Snow(int nX, int nY, int range, int up_time, int const_time, int down_time, int intensity)
{

}


Mission *Mission::Get(int id)
{
    for (auto mis : missions)
    {
        if (mis->id == id)
        {
            return mis;
        }
    }

    return nullptr;
}


void Mission::Destroy()
{
//    for (auto mis : missions)
//    {
//        delete mis;
//    }
}


void Mission::RegisterGoal(int num, pchar text, pchar p1, pchar p2, pchar p3, pchar p4, pchar p5, pchar p6)
{
    LOG_WARNING_HI("Not implemented");
}
