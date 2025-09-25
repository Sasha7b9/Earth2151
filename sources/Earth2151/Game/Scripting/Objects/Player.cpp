// 2025/03/25 13:11:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Objects/Player.h"
#include "Game/Scripting/Objects/Unit.h"


SPlayer::SPlayer(pchar f, pchar n, ScriptReader *r) :
    SObject(f, n, r)
{
}



void SPlayer::EnableBuilding(pchar id, int enable)
{

}


void SPlayer::EnableResearch(pchar id, int enable)
{

}


UnitEx SPlayer::GetUnit(int nX, int nY)
{
    return UnitEx("");
}


UnitEx SPlayer::GetUnit(int nX, int nY, int nZ)
{
    return UnitEx("");
}


void SPlayer::LookAt(int nX, int nY, int nZ, int alpha, int view, int tunnel)
{

}


void SPlayer::SetMilitaryUnitsLimit(int units_limit)
{

}


void SPlayer::SetMoney(int money)
{

}


void SPlayer::SetScriptData(int index_data, int value_data)
{

}


void SPlayer::SetScriptUnit(int index_unit, UnitEx)
{

}
