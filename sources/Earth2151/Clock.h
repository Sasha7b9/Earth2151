// 2025/03/18 15:23:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define UCOUNT_MS  Clock::CountMS()
#define FDELTA_SEC Clock::DeltaSec()
#define IDELTA_MS  Clock::DeltaTime()


namespace Clock
{
    uint CountMS();

    float DeltaSec();

    int DeltaTime();

    void Pause();

    void Resume();
}