// 2025/03/25 12:47:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SUnit.h"


namespace S
{
    class Tank : public Unit
    {
    public:
        Tank(pchar file_name, pchar name, ScriptReader *);
    };
}
