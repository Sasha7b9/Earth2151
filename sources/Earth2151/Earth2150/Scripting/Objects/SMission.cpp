// 2025/03/25 13:10:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SMission.h"


S::Mission::Mission(pchar file_name, pchar name, ScriptReader *_reader) :
    ObjectX(file_name, name, _reader)
{
}


void S::Mission::CallCamera()
{

}


void S::Mission::EnableGoal(int num_goal, int enable)
{

}


S::Player S::Mission::GetPlayer(int num_player)
{
    return S::Player();
}


void S::Mission::RegisterGoal(int num_goal, pchar goal_text)
{

}


void S::Mission::SelectUnit(UnitEx unit, int add_to_selection)
{

}


void S::Mission::SetTimer(int num_timer, int interval)
{

}


void S::Mission::Snow(int nX, int nY, int range, int up_time, int const_time, int down_time, int intensity)
{

}
