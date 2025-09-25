// 2025/03/24 22:18:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Scripting/Objects/Object.h"


class Campaign : public ObjectX
{
public:

    Campaign(pchar f, pchar n, ScriptReader *r) : ObjectX(f, n, r)
    {
    }

    virtual void Update() override;

    virtual bool ExecuteOperatorFunction(OperatorFunction *) override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // It registers mission in campaign script.
    void RegisterMission(int nMissinNum, pchar strMissionLevel, pchar strMissionScript,
        pchar strShortBriefing,                                             // short version of briefing
        pchar nFlags,                                                       // mission flags
        int nLongitude, int nLatitude,                                      // location on Earth
        int nDistanceToBase1, int nDistanceToBase2, int nDistanceToBase3,   //distances to ED, UCS and LC bases(the time needed to get there).
        // user can specified up to 4 mission which may be activated using EnableNextMission function.
        int nNextMission0 = -1, int nNextMission1 = -1, int nNextMission2 = -1, int nNextMission3 = -1);

    void ActivateMissions(int flags_and_value, int activate);

    void ActivateMissionsEq(int flags_and_value, int flags_eq_avlue, int activate);

    void AreMissionsEnabledEq(int flags_and_value, int flags_eq_value);

    void EnableChooseMissionButton(int enable);

    void EnableMission(int num_mission, int enable);

    void LoadMission(int num_world, int num_mission);

    void SetActivePlayerAndWorld(int num_player, int num_world);

    void SetMissionState(int num_missioni, int state);

    // season = [1...7]
    void SetSeason(int season);

    struct Events
    {
        void EndMission(int num_mission, int result);

        void EnableNextMission(int num_mission, int num_next_mission, int enable);

        void StartMission(int num_mission);
    };

    Events events;
};
