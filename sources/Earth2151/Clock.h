// 2025/03/18 15:23:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define UCOUNT_MS  Clock::CountMS()
#define FDELTA_SEC Clock::DeltaSec()
#define IDELTA_MS  Clock::DeltaTime()

#define COUNT_TICKS Clock::CountTicks()


namespace Clock
{
    uint CountMS();

    float DeltaSec();

    int DeltaTime();

    void Pause();

    void Resume();

    // Количество тиков с момента запуска. В одной секунде 20 тиков
    uint CountTicks();
}


// Структура для измерения тиков
struct Counter
{
    Counter();

    void Reset();

    uint Elapsed() const;

    void Start(uint dT);

    bool IsFinished() const;

private:

    uint start;         // Количество тиков, от которого отсчитываем время

    uint time_finished;
};
