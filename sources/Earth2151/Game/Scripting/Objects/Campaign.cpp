// 2025/03/24 22:18:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Objects/Campaign.h"
#include "Game/Scripting/Objects/Mission.h"
#include "Utils/Locale.h"
#include "Game/GamePlayer.h"
#include "Game/Game.h"


bool Campaign::ExecuteOperatorFunction(OperatorFunction *oper)
{
    if (ObjectX::ExecuteOperatorFunction(oper))
    {
        return true;
    }

    if (oper->name_func == "ActivateMissions")
    {
        ActivateMissions(
            oper->ResolveParameterInt(0),
            oper->ResolveParameterInt(1));

        return true;
    }
    else if (oper->name_func == "CreateGamePlayer")
    {
        GamePlayer::Create(
            oper->ResolveParameterInt(0),
            (Race::E)oper->ResolveParameterInt(1),
            (PlayerType::E)oper->ResolveParameterInt(2),
            oper->ResolveParameterStr(3).c_str());

        return true;
    }
    else if (oper->name_func == "EnableChooseMissionButton")
    {
        EnableChooseMissionButton(oper->ResolveParameterInt(0));
        return true;
    }
    else if (oper->name_func == "EnableMission")
    {
        EnableMission(
            oper->ResolveParameterInt(0),
            oper->ResolveParameterInt(1));
        return true;
    }
    else if (oper->name_func == "LoadBase")
    {
        TheGame->LoadBase(
            oper->ResolveParameterInt(0),
            oper->ResolveParameterInt(1),
            oper->ResolveParameterInt(2));

        return true;
    }
    else if (oper->name_func == "RegisterMission")
    {
        RegisterMission(
            oper->ResolveParameterInt(0),           // num
            oper->ResolveParameterStr(1).c_str(),   // level
            oper->ResolveParameterStr(2).c_str(),   // script
            oper->ResolveParameterStr(3).c_str(),   // briefing
            oper->ResolveParameterStr(4),           // flags
            oper->ResolveParameterInt(5),           // longitude
            oper->ResolveParameterInt(6),           // lattitude
            oper->ResolveParameterInt(7),           // d1
            oper->ResolveParameterInt(8),           // d2
            oper->ResolveParameterInt(9),           // d3
            oper->ResolveParameterInt(10),          // next0
            oper->ResolveParameterInt(11),          // next1
            oper->ResolveParameterInt(12),          // next2
            oper->ResolveParameterInt(13));         // next3

        return true;
    }
    else if (oper->name_func == "SetActivePlayerAndWorld")
    {
        SetActivePlayerAndWorld(
            oper->ResolveParameterInt(0),
            oper->ResolveParameterInt(1));

        return true;
    }
    else if (oper->name_func == "SetSeason")
    {
        SetSeason(oper->ResolveParameterInt(0));

        return true;
    }
    else if (oper->name_func == "SetTime")
    {
        ObjectX::SetTime(oper->ResolveParameterInt(0));

        return true;
    }

    LOG_ERROR_HI("Not implemented CALL for \"%s\"", oper->name_func.c_str());

    return false;
}


void Campaign::Update()
{
//    LOG_WRITE(" ");
//    LOG_WRITE("Campaign \"%s\" \"%s\"update", name.c_str(), _L(name.c_str()).c_str());

    ObjectX::Update();
}


void Campaign::RegisterMission(int nMissinNum, pchar strMissionLevel, pchar strMissionScript,
    pchar strShortBriefing, pchar nFlags, int nLongitude, int nLatitude, int nDistanceToBase1, int nDistanceToBase2, int nDistanceToBase3,
    int nNextMission0, int nNextMission1, int nNextMission2, int nNextMission3)
{
   Mission::Get(nMissinNum)->Construct(nMissinNum, strMissionLevel, strMissionScript, strShortBriefing,
        nFlags, nLongitude, nLatitude, nDistanceToBase1, nDistanceToBase2, nDistanceToBase3,
        nNextMission0, nNextMission1, nNextMission2, nNextMission3);
}



void Campaign::ActivateMissions(int flags_and_value, int activate)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, flags_and_value, activate);
}


void Campaign::ActivateMissionsEq(int flags_and_value, int flags_eq_avlue, int activate)
{

}


void Campaign::AreMissionsEnabledEq(int flags_and_value, int flags_eq_value)
{

}


void Campaign::EnableChooseMissionButton(int enable)
{
    LOG_WRITE("\"%s\" %d", __FUNCTION__, enable);
}


void Campaign::EnableMission(int num_mission, int enable)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, num_mission, enable);
}


void Campaign::LoadMission(int num_world, int num_mission)
{

}


void Campaign::SetActivePlayerAndWorld(int num_player, int num_world)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, num_player, num_world);
}


void Campaign::SetMissionState(int num_missioni, int state)
{

}


void Campaign::SetSeason(int season)
{
    LOG_WRITE("\"%s\" %d", __FUNCTION__, season);
}


void Campaign::Events::EndMission(int num_mission, int result)
{

}


void Campaign::Events::EnableNextMission(int num_mission, int num_next_mission, int enable)
{

}


void Campaign::Events::StartMission(int num_mission)
{

}
