// 2025/03/25 12:48:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/STank.h"


namespace S
{
    class Aircraft : public Tank
    {
    public:
        Aircraft(pchar file_name, pchar name, ScriptReader *);
    };
}
