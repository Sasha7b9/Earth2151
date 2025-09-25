// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Scripting/Objects/Object.h"
#include "Game/Scripting/Objects/Unit.h"


class SPlayer : public SObject
{
public:

    static const int MAX_COUNT_PLAYERS = 3;

    SPlayer(pchar file_name = "", pchar name = "", ScriptReader *reader = nullptr);

    void EnableBuilding(pchar id, int enable);

    void EnableResearch(pchar id, int enable);

    int GetMoney();

    int GetMoneyFromFinishedMissions();

    int GetMoneySentToOrbit();

    String<> GetName();

    int GetNumberOfBuildings();
    int GetNumberOfBuildings(int type_building);

    UnitEx GetScriptUnit(int index_unit);

    UnitEx GetUnit(int nX, int nY);
    UnitEx GetUnit(int nX, int nY, int nZ);

    void LookAt(int nX, int nY, int nZ, int alpha, int view, int tunnel);

    void SetMilitaryUnitsLimit(int units_limit);

    void SetMoney(int money);

    void SetScriptData(int index_data, int value_data);

    void SetScriptUnit(int index_unit, UnitEx);
};
