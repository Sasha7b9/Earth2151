// 2025/03/24 22:11:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Objects/SObject.h"


namespace S
{
    // ObjectX - contains functions which are common for campaign and mission object
    class ObjectX : public SObject
    {
    public:

        ObjectX(pchar file_name, pchar name, ScriptReader *_reader) : SObject(file_name, name, _reader) { }

        virtual bool ExecuteOperatorCall(OperatorCall *) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void SendCustomEvent(int param1, int param2, int param3, int param4, int param5);

        void SetAvailableWorlds(int worlds_mask);

        void ShowVideo(pchar video_name);

        void EndGame(pchar outro);
    };
}
