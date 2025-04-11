// 2025/03/25 12:16:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Earth2150/Scripting/Primitives/Block.h"
#include "Earth2150/Scripting/ScriptReader.h"


namespace S
{
    class Campaign;
    struct OperatorCall;

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

        SObject(pchar file_name, pchar name, ScriptReader * = nullptr);

        ~SObject()
        {
            SAFE_DELETE(script);
        }

        Campaign *ToCampaign()
        {
            return (Campaign *)this;
        }

        virtual void Reset();

        virtual void Update();

        virtual bool ExecuteOperatorCall(OperatorCall *);

        static SObject *Create(pchar name_script);

        void TraceD(pchar text);

    protected:

        String<> name;

        ScriptReader *reader;

    private:

        static String<> CreateContent(FileReader &);
    };
}
