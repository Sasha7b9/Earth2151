// 2025/03/25 21:05:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/ScriptEngine.h"
#include "Earth2150/Scripting/ScriptStorage.h"
#include "Earth2151.h"
#include "FileSystem/ListFiles.h"


ScriptEngine *TheScriptEngine = nullptr;

S::Campaign *TheCampaign = nullptr;


ScriptEngine::ScriptEngine() : Global<ScriptEngine>(TheScriptEngine)
{
    new ScriptStorage();

    std::vector<std::string> ignored_files;
    std::vector<std::string> ignored_ext;
    std::vector<std::string> ignored_dirs;

    ListFiles files(RESOURCE_PATH("Scripts").c_str(), &ignored_files, &ignored_ext, &ignored_dirs);

    for (const FileInfo &info : files.files)
    {
        if (!SU::EndsWith(info.name.c_str(), ".ec"))
        {
            continue;
        }

        S::SObject *object = S::SObject::Create(info.name.c_str());

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
