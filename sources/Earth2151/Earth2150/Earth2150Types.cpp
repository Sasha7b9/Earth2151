// 2025/03/06 14:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Earth2150Types.h"
#include "Objects/World/Landscape.h"
#include "Earth2150/Parameters/Models/Abstracts/Entity.h"
#include "Earth2151.h"


namespace Parser
{
    static void Parse(const String<> &, Array<int> &, Array<String<>> &);
}


void Level2150::ReadObjects()
{
    objects.ReadObjects(mis);

    objects.SaveToFileTXT((file_name_mis + ".txt").c_str());
}


void LObjects::ReadObjects(HeapBuffer &buf)
{
    buf.ResetRead();

    uint unknown = 0;

    buf.ReadData(&unknown, 4);

    String<> name_file = ReadString(1, buf);

    buf.ReadData(&unknown, 4);

    uint8 guid[16];
    buf.ReadData(guid, 16);

    if (unknown < 256)                                          // Так читается большинство оригинальных *.mis
    {
        String<> str_unknown = ReadString(4, buf);
        uint8 guid_file[16];
        buf.ReadData(guid_file, 16);
    }
    else
    {
        buf.ReadData(&unknown, 4);

        int pointer = buf.pointer_read;

        buf.ReadData(&unknown, 4);

        buf.pointer_read = pointer;

        if(unknown < 256)
        {                                                   // Так читаются некоторые оригинальные *.mis вроде !113
            String<> str_unknw = ReadString(4, buf);
        }
        else
        {                                                   // Так читаются файлы, созданные в редакторе
            uint8 g[16];
            buf.ReadData(g, 16);
        }
    }

    uint8 water_type;

    buf.ReadData(&water_type, sizeof(water_type));

    buf.ReadData(&unknown, 4);

    int width = 0;
    int height = 0;

    buf.ReadData(&width, 2);
    buf.ReadData(&height, 2);

    buf.ReadData(&unknown, 4);

    uint8 bytes[96];

    buf.ReadData(bytes, 96);

    unknown = 0;

    buf.ReadData(&unknown, 2);

    ReadMarkers(buf);

    while (!buf.FullyReaded())
    {
        LObject object;

        ReadUnit(buf, object);

        if (!object.IsEmpty())
        {
            objects.AppendArrayElement(object);
        }
    }
}


void LObjects::ReadMarkers(HeapBuffer &buf)
{
    uint number_markers = 0;

    buf.ReadData(&number_markers, 2);

    for (uint i = 0; i < number_markers; i++)
    {
        uint number = 0;
        uint16 startX = 0;
        uint16 startY = 0;

        buf.ReadData(&number, sizeof(number));
        buf.ReadData(&startX, sizeof(startX));
        buf.ReadData(&startY, sizeof(startY));
    }
}


void LObjects::ReadUnit(HeapBuffer &buf, LObject &object)
{
    static int counter = 0;
    counter++;

    object.description = ReadString(4, buf);

    if (!object.IsEmpty())
    {
        Parser::Parse(object.description, object.values, object.strings);

        char symbol = object.description.c_str()[0];

        object._position = { (float)object.values[0], Landscape::GetSize().y - (float)object.values[1] - 2 };       // \todo Непонятно, откуда это смещение. Добавлено, чтобы в текстуры попасть

        object.rotation_rad = GTR(-(object.values[3] / 256.0f * 360.0f) + 90.0f);

        if (std::isdigit(symbol) || symbol == '-')
        {
            object.underground = (object.values[2] != 0);
        }
        else                                                                                        // для bridge
        {
        }
    }
}


String<> LObjects::ReadString(int size_size, HeapBuffer &buf)
{
    uint size = 0;

    buf.ReadData(&size, size_size);

    HeapBuffer string(128);

    for (uint i = 0; i < size; i++)
    {
        char symbol = 0;

        buf.ReadData(&symbol, 1);

        string.WriteData(&symbol, 1);
    }

    size = 0;

    string.WriteData(&size, 1);

    return String<>((char *)string.data);
}


void LObjects::SaveToFileTXT(pchar path)
{
    File file;

    if (file.OpenFile(path, kFileCreate) != kFileOkay)
    {
        LOG_ERROR("Can not create file %s", path);
        return;
    }

    for (int i = 0; i < objects.GetArrayElementCount(); i++)
    {
        file.WriteString(objects[i].CreateFullDescription(i));
    }
}


void LObject::JobCreateObject(Job *, void *cookie)
{
    static Mutex mutex;

    static int counter = 0;

    mutex.Acquire();

    LObject *object = (LObject *)cookie;

    String<> name = object->strings[0];

    if (name == "bridge")           // Видимо, это мост
    {
        name = object->strings[1];
    }

    Entity *entity = TheEarth2151->param->GetEntity(name);

    if (entity)
    {
        Entity::CreateModel(entity, *object);
    }
    else
    {
        LOG_ERROR("Not exist entity %s", name.c_str());
    }

    mutex.Release();
}


String<> LObject::CreateFullDescription(int num)
{
    String<> result{ Text::Format(" Object %3d : %s\n", num, description.c_str()) };

    String<> full_string;

    for (int i = 0; i < strings.GetArrayElementCount(); i++)
    {
        full_string.AppendString(strings[i]);
        full_string.AppendString(" ");
    }

    String<> full_values;

    for (int i = 0; i < values.GetArrayElementCount(); i++)
    {
        full_values.AppendString(Text::Format("%d", values[i]));
        full_values.AppendString(" ");
    }

    result.AppendString(Text::Format("name        : %s\n", full_string.c_str()));
    result.AppendString(Text::Format("values      : %s\n", full_values.c_str()));
    result.AppendString(Text::Format("pos         : { %d, %d }\n", (int)_position.x, (int)_position.y));
    result.AppendString(Text::Format("underground : %s\n", underground ? "yes" : "no"));
    result.AppendString(Text::Format("rotation    : %f\n", RTG(rotation_rad)));

    return result;
}


void Parser::Parse(const String<> &line, Array<int> &values, Array<String<>> &strings)
{
    Array<String<>> words;

    SU::SplitToWords(line, words, ", #");

    for (int i = 0; i < words.GetArrayElementCount(); i++)
    {
        pchar str = words[i].c_str();

        char symbol = str[0];

        if (std::isdigit(symbol) || symbol == '-')
        {
            values.AppendArrayElement(std::atoi(str));
        }
        else
        {
            strings.AppendArrayElement(words[i]);
        }
    }

    words.PurgeArray();
}
