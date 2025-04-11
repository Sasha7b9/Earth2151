// 2025/03/25 13:11:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SPlayer.h"
#include "Earth2150/Scripting/Objects/SUnitEx.h"


S::Player::Player(pchar file_name, pchar name, ScriptReader *_reader) :
    SObject(file_name, name, _reader)
{
}



void S::Player::EnableBuilding(pchar id, int enable)
{

}


void S::Player::EnableResearch(pchar id, int enable)
{

}


S::UnitEx S::Player::GetUnit(int nX, int nY)
{
    return UnitEx("");
}


S::UnitEx S::Player::GetUnit(int nX, int nY, int nZ)
{
    return UnitEx("");
}


void S::Player::LookAt(int nX, int nY, int nZ, int alpha, int view, int tunnel)
{

}


void S::Player::SetMilitaryUnitsLimit(int units_limit)
{

}


void S::Player::SetMoney(int money)
{

}


void S::Player::SetScriptData(int index_data, int value_data)
{

}


void S::Player::SetScriptUnit(int index_unit, UnitEx)
{

}
