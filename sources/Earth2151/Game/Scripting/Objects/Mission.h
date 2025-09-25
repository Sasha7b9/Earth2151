// 2025/03/25 12:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Scripting/Objects/Player.h"


class Mission : public ObjectX
{
public:

    Mission(pchar f, pchar n, ScriptReader *);

    void Construct(int nMissinNum, pchar strMissionLevel, pchar strMissionScript,
        pchar strShortBriefing, pchar nFlags, int nLongitude, int nLatitude, int nDistanceToBase1, int nDistanceToBase2, int nDistanceToBase3,
        int nNextMission0 = -1, int nNextMission1 = -1, int nNextMission2 = -1, int nNextMission3 = -1);

    virtual void Update() override;

    virtual bool ExecuteOperatorFunction(OperatorFunction *) override;

    // It registers goal or changes previously registered goal.
    void RegisterGoal(int nGoalNum, pchar text, pchar p1, pchar p2, pchar p3, pchar p4, pchar p5, pchar p6);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CallCamera();

    void EnableGoal(int num_goal, int enable);

    void EnableNextMission(int num_mission, int enable);

    int GetGoalState(int num_goal);

    int GetMissionTime();

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

    int id;
    String<> file_level;
    String<> file_script;
    String<> briefing;
    String<> flags;
    int longitude;
    int latitude;
    int d1;
    int d2;
    int d3;
    int id_next1;
    int id_next2;
    int id_next3;
    int id_next4;

    static Mission *Get(int num);

    static void Destroy();

private:

    static std::vector<Mission *> missions;
};
