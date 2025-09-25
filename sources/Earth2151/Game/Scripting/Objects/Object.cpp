// 2025/03/25 12:16:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Objects/Object.h"
#include "Utils/StackBuffer.h"
#include "Game/Scripting/Objects/Player.h"
#include "Game/Scripting/Objects/Campaign.h"
#include "Game/Scripting/Objects/Mission.h"
#include "Game/Scripting/Objects/Unit.h"
#include "Earth2151.h"


SObject::SObject(pchar f, pchar n, ScriptReader *r) :
    file_name(f), name(n), reader(r)
{
    script = reader->CreateScript(this);
}


SObject *SObject::Create(pchar name_script)
{
    File file;

    if (file.OpenFile(name_script, kFileReadOnly) != kFileOkay)
    {
        LOG_ERROR_HI("Can not open file \"%s\"", name_script);

        return nullptr;
    }

    FileReader f_reader(&file);

    String<> content = CreateContent(f_reader);

    File out_file;

    if (out_file.OpenFile(String<>(name_script) + ".txt", kFileCreate) != kFileOkay)
    {
        LOG_ERROR_HI("Can not create output file");

        return nullptr;
    }

    out_file.WriteString(content.c_str());

    ScriptReader reader(content);

    String<> type = reader.ReadString();

    String<> name = reader.ReadStringQuotes();

//    LOG_WRITE("%d %s %s", counter, name_script, name.c_str());

    if      (type == "aircraft")    return new UAircraft(name_script, name, &reader);
    else if (type == "builder")     return new UBuilder(name_script, name, &reader);
    else if (type == "campaign")    return new Campaign(name_script, name, &reader);
    else if (type == "carrier")     return new UCarrier(name_script, name, &reader);
    else if (type == "civil")       return new UCivil(name_script, name, &reader);
    else if (type == "harvester")   return new UHarvester(name_script, name, &reader);
    else if (type == "mission")     return new Mission(name_script, name, &reader);
    else if (type == "platoon")     return new OPlatoon(name_script, name, &reader);
    else if (type == "player")      return new SPlayer(name_script, name, &reader);
    else if (type == "repairer")    return new URepairer(name_script, name, &reader);
    else if (type == "sapper")      return new USapper(name_script, name, &reader);
    else if (type == "supplier")    return new USupplier(name_script, name, &reader);
    else if (type == "tank")        return new UTank(name_script, name, &reader);

    return nullptr;
}


String<> SObject::CreateContent(FileReader &reader)
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


void SObject::Reset()
{
    script->Reset();
}


void SObject::Update()
{
    script->Execute();
}


bool SObject::ExecuteOperatorFunction(OperatorFunction *)
{
    return false;
}
