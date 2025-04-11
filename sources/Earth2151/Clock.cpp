// 2025/03/18 15:23:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Clock.h"
#include "Earth2151.h"


namespace Clock
{
    static uint count_MS_start_pause = 0;           // Это время будет передаваться во время паузы
    static uint count_MS_start_pause_engine = 0;    // Используется для расчёта delta
    static uint count_MS_delta = 0;                 // Это время отнимаем от возвращаемого движком, чтобы получить правильное
}


uint Clock::CountMS()
{
    if (TheEarth2151->InPaused())
    {
        return count_MS_start_pause;
    }

    return TheTimeMgr->GetMillisecondCount() - count_MS_delta;
}


float Clock::DeltaSec()
{
    if (TheEarth2151->InPaused())
    {
        return 0.0f;
    }

    return TheTimeMgr->GetDeltaSeconds();
}


int Clock::DeltaTime()
{
    if (TheEarth2151->InPaused())
    {
        return 0;
    }

    return TheTimeMgr->GetDeltaTime();
}


void Clock::Pause()
{
    count_MS_start_pause = UCOUNT_MS;
    count_MS_start_pause_engine = TheTimeMgr->GetMillisecondCount();
}


void Clock::Resume()
{
    count_MS_delta += TheTimeMgr->GetMillisecondCount() - count_MS_start_pause_engine;
}


uint Clock::CountTicks()
{
    return UCOUNT_MS / 50;
}


Counter::Counter()
{
    Reset();
}


void Counter::Reset()
{
    start = COUNT_TICKS;
}


uint Counter::Elapsed() const
{
    return COUNT_TICKS - start;
}


void Counter::Start(uint dT)
{
    time_finished = COUNT_TICKS + dT;
}


bool Counter::IsFinished() const
{
    return COUNT_TICKS >= time_finished;
}
