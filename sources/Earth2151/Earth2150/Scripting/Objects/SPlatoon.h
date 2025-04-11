// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SCommandObject.h"


namespace S
{
    class Platoon : public CommandObject
    {
    public:
        Platoon(pchar file_name, pchar name, ScriptReader *);
    };
}
