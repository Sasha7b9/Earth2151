// 2025/03/25 21:05:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/ScriptEngine.h"
#include "Game/Scripting/ScriptStorage.h"
#include "Earth2151.h"
#include "FileSystem/ListFiles.h"


ScriptEngine *TheScriptEngine = nullptr;

Campaign *TheCampaign = nullptr;

Mission *TheMission = nullptr;


ScriptEngine::ScriptEngine() : Global<ScriptEngine>(TheScriptEngine)
{
    new ScriptStorage();

    std::vector<std::string> ignored_files;
    std::vector<std::string> ignored_ext;
    std::vector<std::string> ignored_dirs;

    String<> resource = RESOURCE_PATH("Scripts");

    ListFiles files(resource.c_str(), &ignored_files, &ignored_ext, &ignored_dirs);

    for (const FileInfo &info : files.files)
    {
        if (!SU::EndsWith(info.name.c_str(), ".ec"))
        {
            continue;
        }

        SObject *object = SObject::Create(info.name.c_str());

        if (object)
        {
            TheScriptStorage->objects.InsertMapElement(object);
        }
    }
}


ScriptEngine::~ScriptEngine()
{
    SAFE_DELETE(TheScriptStorage);
}
