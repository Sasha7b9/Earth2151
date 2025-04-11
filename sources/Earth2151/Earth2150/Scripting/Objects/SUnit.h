// 2025/03/25 12:25:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SCommandObject.h"


namespace S
{
    class Unit : public CommandObject
    {
    public:
        Unit(pchar file_name, pchar name, ScriptReader *);

        int IsLive();
    };
}
