// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObjectX.h"
#include "Earth2150/Scripting/Objects/SPlayer.h"


namespace S
{
    class Mission : public ObjectX
    {
    public:

        Mission(pchar file_name, pchar name, ScriptReader *);

        void CallCamera();

        void EnableGoal(int num_goal, int enable);

        void EnableNextMission(int num_mission, int enable);

        int GetGoalState(int num_goal);

        int GetMissionTime();

        Player GetPlayer(int num_player);

        void RegisterGoal(int num_goal, pchar goal_text);

        void SelectUnit(UnitEx unit, int add_to_selection);

        void SetGoalState(int num_goal, int state);

        void SetTimer(int num_timer, int interval);

        void Snow(int nX, int nY, int range, int up_time, int const_time, int down_time, int intensity);

        struct Events
        {
            void Timer0();

            int CustomEvent0(int param1, int param2, int param3, int param4);
        };

        Events events;
    };
}
