// 2025/03/25 12:23:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObject.h"


namespace S
{
    class CommandObject : public SObject
    {
    public:
        CommandObject(pchar file_name, pchar name, ScriptReader *);
    };
}
