// 2022/09/14 14:35:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define DISPLAY_LAST_ERROR() System::DisplayLastError(__FILE__, __LINE__)

namespace System
{
    void DisplayLastError(pchar file, int line);

    // Записать в лог стек вызовов
    void LogBacktrace();
}
