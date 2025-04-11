// 2025/04/06 01:12:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Utils/Users.h"
#include "FileSystem/FileSystem.h"
#include "Earth2151.h"


namespace Users
{
    static Array<String<>> names;

    static int pointer = 0;             // Используется для чтения имён

    static void ReadNames();

    static bool FileMapFilter(pchar name, uint flags, const void *);
}


String<> Users::GetFirst()
{
    ReadNames();

    pointer = 0;

    return GetNext();
}


String<> Users::GetNext()
{
    if (pointer < names.GetArrayElementCount())
    {
        return names[pointer++];
    }

    return "";
}


bool Users::Exist(const String<> &name)
{
    for (auto &n : names)
    {
        if (n == name)
        {
            return true;
        }
    }

    return false;
}


bool Users::Create(const String<> &name)
{
    if (Exist(name))
    {
        return false;
    }

    String<> dir = RESOURCE_PATH("Players/") + name + "." + TypeCampaign::Name();

    FS::CreateDirectory(dir.c_str());

    return true;
}


bool Users::Remove(const String<> &name)
{
    String<> dir = RESOURCE_PATH("Players/") + name + "." + TypeCampaign::Name();

    FS::RemoveDirectory(dir.c_str());

    ReadNames();

    return true;
}


bool Users::Enter(const String<> &)
{
    return false;
}


void Users::ReadNames()
{
    names.ClearArray();

    Map<FileReference> map;

    TheFileMgr->BuildFileMap(RESOURCE_PATH("Players"), &map, FileMapFilter);

    FileReference *ref = map.GetFirstMapElement();

    while (ref)
    {
        Array<String<>> words;

        SU::SplitToWords(ref->fileName, words, ".");

        names.AppendArrayElement(words[0]);

        ref = ref->GetNextMapElement();
    }
}


bool Users::FileMapFilter(pchar name, uint flags, const void *)
{
    if (!(flags & kFileDirectory))
    {
        return false;
    }

    int length = Text::GetTextLength(name);

    String<> suffix{ String<>(".") + TypeCampaign::Name() };

    if (length <= suffix.GetStringLength())
    {
        return false;
    }

    return Text::CompareTextCaseless(&name[MaxZero(length - suffix.GetStringLength())], suffix.c_str());
}
