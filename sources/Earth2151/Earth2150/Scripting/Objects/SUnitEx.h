// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SUnit.h"


namespace S
{
    class UnitEx : public Unit
    {
    public:
        UnitEx(pchar file_name, pchar name = "", ScriptReader *_reader = nullptr) : Unit(file_name, name, _reader) { }
        void CommandMove(int nX, int nY, int nZ);
    };
}
