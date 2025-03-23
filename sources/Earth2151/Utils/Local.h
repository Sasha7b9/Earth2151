// 2025/03/22 09:07:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define _L(x)    Local::GetString(x)


namespace Local
{
    void Init();

    void DeInit();

    String<> GetString(pchar);
}
