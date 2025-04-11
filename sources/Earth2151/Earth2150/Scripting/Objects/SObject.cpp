// 2025/03/25 12:16:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Objects/SObject.h"
#include "Utils/StackBuffer.h"
#include "Earth2150/Scripting/Objects/SPlayer.h"
#include "Earth2150/Scripting/Objects/SCampaign.h"
#include "Earth2150/Scripting/Objects/SMission.h"
#include "Earth2150/Scripting/Objects/STank.h"
#include "Earth2150/Scripting/Objects/SBuilder.h"
#include "Earth2150/Scripting/Objects/SCarrier.h"
#include "Earth2150/Scripting/Objects/SCivil.h"
#include "Earth2150/Scripting/Objects/SHarvester.h"
#include "Earth2150/Scripting/Objects/SAircraft.h"
#include "Earth2150/Scripting/Objects/SPlatoon.h"
#include "Earth2150/Scripting/Objects/SRepairer.h"
#include "Earth2150/Scripting/Objects/SSapper.h"
#include "Earth2150/Scripting/Objects/SSupplier.h"
#include "Earth2151.h"


S::SObject::SObject(pchar _file_name, pchar _name, ScriptReader *_reader) :
    file_name(_file_name), name(_name), reader(_reader)
{
    if (RESOURCE_PATH("Scripts/campaigns/Lc/CampaignLC.ec") == _file_name)
    {
        file_name = file_name;
    }

    script = reader->CreateScript(this);
}


S::SObject *S::SObject::Create(pchar name_script)
{
    File file;

    if (file.OpenFile(name_script, kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR("Can not open file \"%s\"", name_script);

        return nullptr;
    }

    FileReader f_reader(&file);

    String<> content = CreateContent(f_reader);

    File out_file;

    if (out_file.OpenFile(String<>(name_script) + ".txt", kFileCreate) != kFileOkay)
    {
        LOG_ERROR("Can not create output file");

        return nullptr;
    }

    out_file.WriteString(content.c_str());

    ScriptReader reader(content);

    String<> type = reader.ReadString();

    String<> name = reader.ReadStringQuotes();

//    LOG_WRITE("%d %s %s", counter, name_script, name.c_str());

    if      (type == "aircraft")    return new S::Aircraft(name_script, name, &reader);
    else if (type == "builder")     return new S::Builder(name_script, name, &reader);
    else if (type == "campaign")    return new S::Campaign(name_script, name, &reader);
    else if (type == "carrier")     return new S::Carrier(name_script, name, &reader);
    else if (type == "civil")       return new S::Civil(name_script, name, &reader);
    else if (type == "harvester")   return new S::Harvester(name_script, name, &reader);
    else if (type == "mission")     return new S::Mission(name_script, name, &reader);
    else if (type == "platoon")     return new S::Platoon(name_script, name, &reader);
    else if (type == "player")      return new S::Player(name_script, name, &reader);
    else if (type == "repairer")    return new S::Repairer(name_script, name, &reader);
    else if (type == "sapper")      return new S::Sapper(name_script, name, &reader);
    else if (type == "supplier")    return new S::Supplier(name_script, name, &reader);
    else if (type == "tank")        return new S::Tank(name_script, name, &reader);

    return nullptr;
}


String<> S::SObject::CreateContent(FileReader &reader)
{
    StackBuffer <1024 * 1024>buffer;

    bool del_spaces = true;

    while (!reader.IsEOF())
    {
begin_while:
        char symbol = (char)reader.ReadByte();

        if (symbol == '/')
        {
            char symbol2 = (char)reader.ReadByte();

            if (symbol2 == '/')
            {
                while (symbol2 != 0x0A)
                {
                    symbol2 = reader.ReadByte();
                }

                del_spaces = true;
                continue;
            }
            else if (symbol2 == '*')
            {
                while (true)
                {
                    if (symbol == '*' && symbol2 == '/')
                    {
                        goto begin_while;
                    }
                    else
                    {
                        symbol = symbol2;
                        symbol2 = (char)reader.ReadByte();
                    }
                }
            }
            else
            {
                buffer.Append(symbol);
                buffer.Append(symbol2);
            }
        }

        if (symbol == 0x0D || symbol == 0x0A)
        {
            del_spaces = true;
            buffer.Append(' ');
            continue;
        }

        if (symbol == ' ' && del_spaces)
        {
            continue;
        }

        buffer.Append(symbol);

        if (symbol != ' ')
        {
            del_spaces = false;
        }
    }

    buffer.Append(' ');

    buffer.Append('\0');

    return String<>((char *)buffer.data);
}


void S::SObject::Reset()
{
    script->Reset();
}


void S::SObject::Update()
{
    script->Execute();
}


bool S::SObject::ExecuteOperatorCall(OperatorCall *)
{
    return false;
}
