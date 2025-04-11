// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/STank.h"


namespace S
{
    class Builder : public Tank
    {
    public:
        Builder(pchar file_name, pchar name, ScriptReader *);
    };
}
