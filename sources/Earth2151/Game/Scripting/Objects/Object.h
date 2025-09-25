// 2025/03/25 12:16:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Game/Scripting/Primitives/Block.h"
#include "Game/Scripting/ScriptReader.h"


class Campaign;
class Mission;

class SObject : public MapElement<SObject>
{
public:

    typedef String<> KeyType;

    String<> file_name;

    Script *script = nullptr;

    KeyType GetKey()
    {
        return file_name;
    }

    SObject(pchar f, pchar n, ScriptReader * = nullptr);

    ~SObject()
    {
        SAFE_DELETE(script);
    }

    Campaign *ToCampaign()
    {
        return (Campaign *)this;
    }

    Mission *ToMission()
    {
        return (Mission *)this;
    }

    virtual void Reset();

    virtual void Update();

    virtual bool ExecuteOperatorFunction(OperatorFunction *);

    static SObject *Create(pchar name_script);

    void TraceD(pchar text);

protected:

    String<> name;

    ScriptReader *reader;

private:

    static String<> CreateContent(FileReader &);
};


class CommandObject : public SObject
{
public:
    CommandObject(pchar f, pchar n, ScriptReader *r) :
        SObject(f, n, r)
    {
    }
};


class OPlatoon : public CommandObject
{
public:
    OPlatoon(pchar f, pchar n, ScriptReader *r) :
        CommandObject(f, n, r)
    {
    }
};



// ObjectX - contains functions which are common for campaign and mission object
class ObjectX : public SObject
{
public:

    ObjectX(pchar f, pchar n, ScriptReader *r) : SObject(f, n, r)
    {
    }

    virtual bool ExecuteOperatorFunction(OperatorFunction *) override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void SendCustomEvent(int param1, int param2, int param3, int param4, int param5);

    void SetAvailableWorlds(int worlds_mask);

    void ShowVideo(pchar video_name);

    void EndGame(pchar outro);

    // It changes time of day. Using this function in the middle of the mission or campaign is not recomended.
    // It is usualy the best idea to set up it at the beginning and left unchanged to the end of game.
    void SetTime(int nDayTick);                     // page 15
};

