// 2025/03/25 21:17:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/ScriptStorage.h"


ScriptStorage *TheScriptStorage = nullptr;


ScriptStorage::ScriptStorage() : Global<ScriptStorage>(TheScriptStorage)
{

}


ScriptStorage::~ScriptStorage()
{
    objects.PurgeMap();
}


S::SObject *ScriptStorage::GetObject(pchar file_name)
{
    return objects.FindMapElement(file_name);
}
