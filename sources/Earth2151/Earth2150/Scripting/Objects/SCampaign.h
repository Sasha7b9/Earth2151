// 2025/03/24 22:18:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObjectX.h"


namespace S
{
    class Campaign : public ObjectX
    {
    public:

        Campaign(pchar file_name, pchar name, ScriptReader *);

        virtual void Update() override;

        virtual bool ExecuteOperatorCall(OperatorCall *) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
}
