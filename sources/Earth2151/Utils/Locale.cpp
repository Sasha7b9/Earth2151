// 2025/03/22 09:09:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Utils/Locale.h"
#include "Earth2151.h"
#include "Interface/Menu/Menu.h"
#include "FileSystem/FileJSON.h"
#include <codecvt>
#include <string>

/*
*   C4 Engine работает с UTF-8
*   В Lost Souls символы хранятся в Unicode.
*   В предыдущих версиях символы занимают один байт. Для русского языка это Win-1251
*/


namespace Local
{
    // Эта функция подхватывает также и польский язык
    static std::string cp1251_to_utf8(pchar);

    std::string wcharToUtf8(const std::wstring &wstr)
    {
        // Создаем объект для преобразования
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        // Преобразуем wstring в string
        return converter.to_bytes(wstr);
    }

    struct StructLocal : public MapElement<StructLocal>
    {
        StructLocal(String<> &_key, String<> &_value) : key(_key), value(_value)  {  }

        typedef String<> KeyType;

        String<> key;                   // Название строки
        String<> value;                 // Сама строка

        KeyType GetKey()
        {
            return key;
        }
    };

    static Array<Map<StructLocal> * > strings;

    static FileJSON json;
}


void Local::Init()
{
    if (!json.Load(DATA_PATH("Language/language.json").c_str()))
    {
        LOG_ERROR("Not parsed language.json");
    }

    if (strings.GetArrayElementCount() == 0)
    {
        for (int i = 0; i < Language::Count; i++)
        {
            strings.AppendArrayElement(new Map<StructLocal>());
        }
    }

    if (strings[Language().Current()]->GetMapElementCount() == 0)
    {
        File file;

        String<> name_file{ RESOURCE_PATH("Language/language.lan") };

        if (file.OpenFile(name_file) != kFileOkay)
        {
            LOG_ERROR("Can not open language file %s", name_file.c_str());
        }

        FileReader reader{ &file };

        alignas(wchar_t) char buffer[8 * 1024];

        reader.ReadBuffer(buffer, 4);      // Просто откидываем первые байты

        bool is_unicode = reader.ReadUInt() == 2;

        reader.ReadInt();

        // А сейчас пошли слова

        while (!reader.IsEOF())
        {
            uint size = reader.ReadUInt();

            reader.ReadBuffer(buffer, size);

            buffer[size] = '\0';

            String<> key{ buffer };

            size = reader.ReadUInt();

            reader.ReadBuffer(buffer, is_unicode ? 2 * size : size);

            buffer[is_unicode ? (size * 2) : size] = '\0';

            if (is_unicode)
            {
                buffer[size * 2 + 1] = '\0';
            }

            String<> value{ (is_unicode ?
                wcharToUtf8((wchar_t *)buffer).c_str() :
                cp1251_to_utf8(buffer).c_str()) };

            strings[Language().Current()]->InsertMapElement(new StructLocal(key, value));
        }
    }

    EMenu::Localize();
}


void Local::DeInit()
{
    json.Unload();

    for (int i = 0; i < strings.GetArrayElementCount(); i++)
    {
        strings[i]->PurgeMap();
        delete strings[i];
    }
}


String<> Local::GetString(pchar str)
{
    {
        // Сначала пытаемся извлечь строку из json

        Array<String<>> values;

        if (json.GetVectorStrings(str, values))
        {
            if (values.GetArrayElementCount() >= (Language().Current() + 1))
            {
                return String<> (values[Language().Current()].c_str());
            }
        }
    }


    StructLocal *elem = strings[Language().Current()]->FindMapElement(String<>{str});

    if (elem)
    {
        if (elem->value[0] == '0')      // Если нет значения для данного языка
        {
            elem = strings[Language::English]->FindMapElement(String<>(str));

            if (elem)
            {
                return elem->value;
            }
        }

        return elem->value;
    }


    return String<>(str);
}


std::string Local::cp1251_to_utf8(const char *str)
{
#ifdef WIN32

    std::string res;
    int result_u, result_c;
    result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (!result_u)
    {
        return 0;
    }
    wchar_t *ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
    {
        delete[] ures;
        return 0;
    }
    result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0); //-V575
    if (!result_c)
    {
        delete[] ures;
        return 0;
    }
    char *cres = new char[result_c];
    if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0))
    {
        delete[] cres;
        return 0;
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;

#else

    return std::string(str);

#endif
}
