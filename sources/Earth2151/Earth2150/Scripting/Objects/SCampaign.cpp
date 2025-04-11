// 2025/03/24 22:18:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SCampaign.h"
#include "Utils/Locale.h"
#include "Game/Mission.h"
#include "Game/GamePlayer.h"
#include "Game/Game.h"


S::Campaign::Campaign(pchar file_name, pchar name, ScriptReader *_reader) :
    ObjectX(file_name, name, _reader)
{
}


bool S::Campaign::ExecuteOperatorCall(OperatorCall *oper)
{
    if (ObjectX::ExecuteOperatorCall(oper))
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
        Mission::Register(
            oper->ResolveParameterInt(0),           // num
            oper->ResolveParameterStr(1).c_str(),   // level
            oper->ResolveParameterStr(2).c_str(),   // script
            oper->ResolveParameterStr(3).c_str(),   // briefing
            oper->ResolveParameterInt(4),           // flags
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

    LOG_ERROR("Not implemented CALL for \"%s\"", oper->name_func.c_str());

    return false;
}


void S::Campaign::Update()
{
//    LOG_WRITE(" ");
//    LOG_WRITE("Campaign \"%s\" \"%s\"update", name.c_str(), _L(name.c_str()).c_str());

    ObjectX::Update();
}


void S::Campaign::ActivateMissions(int flags_and_value, int activate)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, flags_and_value, activate);
}


void S::Campaign::ActivateMissionsEq(int flags_and_value, int flags_eq_avlue, int activate)
{

}


void S::Campaign::AreMissionsEnabledEq(int flags_and_value, int flags_eq_value)
{

}


void S::Campaign::EnableChooseMissionButton(int enable)
{
    LOG_WRITE("\"%s\" %d", __FUNCTION__, enable);
}


void S::Campaign::EnableMission(int num_mission, int enable)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, num_mission, enable);
}


void S::Campaign::LoadMission(int num_world, int num_mission)
{

}


void S::Campaign::SetActivePlayerAndWorld(int num_player, int num_world)
{
    LOG_WRITE("\"%s\" %d %d", __FUNCTION__, num_player, num_world);
}


void S::Campaign::SetMissionState(int num_missioni, int state)
{

}


void S::Campaign::SetSeason(int season)
{
    LOG_WRITE("\"%s\" %d", __FUNCTION__, season);
}


void S::Campaign::Events::EndMission(int num_mission, int result)
{

}


void S::Campaign::Events::EnableNextMission(int num_mission, int num_next_mission, int enable)
{

}


void S::Campaign::Events::StartMission(int num_mission)
{

}
