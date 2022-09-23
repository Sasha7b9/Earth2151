// 2022/09/12 14:12:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Text/String.h"


struct Time
{
    int sec;
    int min;
    int hour;

    // Отнять количество минут
    void SubMin(int);

    String<> ToString() const;

    int ToSec() const;

    bool operator==(const Time &rhs) { return (rhs.sec == sec) && (rhs.min == min) && (rhs.hour == hour); }

    const Time operator-(const Time &rhs);
};


namespace Clock
{
    void Update();

    Time &CurrentTime();
}
