// 2025/03/05 08:50:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Parameters/Parameters2150.h"
#include "Utils/StringUtils.h"


Parameters2150::Parameters2150(pchar _file_name) :
    file_name(_file_name)
{
    File file;

    if (file.OpenFile(file_name.c_str(), kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR("Can not open file \"%s\"", file_name.c_str());

        return;
    }

    FileReader reader(&file);

    if (!header.Fill(reader))
    {
        LOG_ERROR("Wrong parameter file \"%s\"", file_name.c_str());

        return;
    }

    if (ReadGroups(reader))
    {
        ReadResearch(reader);
    }
}


void Parameters2150::Destroy()
{
    for (EntityGroup &group : groups)
    {
        group.Destroy();
    }

    groups.PurgeArray();
}


bool Parameters2150::ReadGroups(FileReader &reader)
{
    uint group_count = reader.ReadUInt();

    reader.ReadUInt();

    for (uint i = 0; i < group_count; i++)
    {
        EntityGroup entity_groupd;

        groups.AppendArrayElement(entity_groupd);

        if (!groups[groups.GetArrayElementCount() - 1].Read(reader))
        {
            LOG_ERROR("Dont read group %d from %d", i + 1, group_count);
            return false;
        }
    }

    return true;
}


void Parameters2150::ReadResearch(FileReader &reader)
{
    uint research_count = reader.ReadUInt();

    reader.ReadUInt();

    for (uint i = 0; i < research_count; i++)
    {
        Research research;

        researches.AppendArrayElement(research);

        researches[researches.GetArrayElementCount() - 1].Read(reader);
    }
}


bool EntityGroup::Read(FileReader &reader)
{
    faction.value = (Faction::E)reader.ReadUInt();
    group_type.value = (EntityGroupType::E)reader.ReadUInt();
    uint group_size = reader.ReadUInt();

    for (uint i = 0; i < group_size; i++)
    {
        Entity *entity = Entity::Create(reader, group_type.value);

        if (!entity)
        {
            return false;
        }

        entities.AppendArrayElement(entity);
    }

    return true;
}


void Parameters2150::Save()
{
    String<> file_txt = file_name + ".txt";

    File file;

    if (file.OpenFile(file_txt.c_str(), kFileCreate) != kFileOkay)
    {
        LOG_ERROR("Can not create file %s", file_txt.c_str());
        return;
    }

    for (int i = 0; i < groups.GetArrayElementCount(); i++)
    {
        groups[i].Save(i, file);
    }
}


void EntityGroup::Save(int num, File &file)
{
    file.WriteString(Text::Format("    Group %3d:                            %s     %s", num, faction.ToString(), group_type.ToString()));

    for (int i = 0; i < entities.GetArrayElementCount(); i++)
    {
        entities[i]->Save(i, file);
    }

    file.WriteString(" ");
}


Entity *Parameters2150::GetEntity(pchar name)
{
    uint64 time = TheTimeMgr->GetMicrosecondCount();

    for (EntityGroup &group : groups)
    {
        Entity *entity = group.GetEntity(name);

        if (entity)
        {
            time_find_us += TheTimeMgr->GetMicrosecondCount() - time;

            return entity;
        }
    }

    time_find_us += TheTimeMgr->GetMicrosecondCount() - time;

    return nullptr;
}
