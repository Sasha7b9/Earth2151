// 2025/03/25 12:28:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SUnit.h"


namespace S
{
    class Civil : public Unit
    {
    public:
        Civil(pchar file_name, pchar name, ScriptReader *);
    };
}

