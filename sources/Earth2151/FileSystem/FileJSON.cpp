// 2022/09/14 14:34:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/FileJSON.h"
#include "Utils/StringUtils.h"


#define CHECK_ON_VALID_STRING   if (!isValid) { LOG_ERROR_HI("Configuration file is not valid"); return nullptr; }
#define CHECK_ON_VALID_INT      if (!isValid) { LOG_ERROR_HI("Configuration file is not valid"); return -1; }
#define CHECK_ON_VALID_BOOL     if (!isValid) { LOG_ERROR_HI("Configuration file is not valid"); return false; }


bool FileJSON::Load(const std::string &_full_path)
{
    full_path = _full_path;

    document = new rapidjson::Document();

    File file;
    if(file.OpenFile(full_path.c_str()) != kFileOkay)
    {
        LOG_ERROR_HI("Can't load configuration file \"%s\"", full_path.c_str());

        return false;
    }

    char buffer[1024 * 1024];

    file.ReadFile(buffer, file.GetFileSize());

    buffer[file.GetFileSize()] = '\0';

    document->Parse(buffer);

    if (document->HasParseError())
    {
        LOG_ERROR_HI("Can't parse configuration file \"%s\". Error %d, offset %u",
            full_path.c_str(), document->GetParseError(), document->GetErrorOffset());

        isValid = false;

        return false;
    }

    isValid = true;

    return true;
}


void FileJSON::Unload()
{
    SAFE_DELETE(document);
}


int FileJSON::GetIntValue(pchar key)
{
    CHECK_ON_VALID_INT;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key);

    if (it != document->MemberEnd() && it->value.IsInt())
    {
        return it->value.GetInt();
    }

    LOG_ERROR_HI("Can't find value for \"%s\"", key);

    return -1;
}


std::vector<float> FileJSON::GetVector3(pchar key, const std::vector<float> &def)
{
    pchar str_value = GetStringValue(key);

    Array<String<>> words;

    SU::SplitToWords(str_value, words, ", ");

    if (words.GetArrayElementCount() != 3)
    {
        LOG_ERROR_HI("Bad vector for key \"%s\"", key);

        return def;
    }

    std::vector<float> result;

    for (int i = 0; i < 3; i++)
    {
        result.push_back((float)SU::ASCIItoFloat(words[i].c_str()));
    }

    return result;
}


bool FileJSON::GetBoolValue(pchar key)
{
    CHECK_ON_VALID_BOOL;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key);

    if (it != document->MemberEnd() && it->value.IsInt())
    {
        return it->value.GetInt() != 0;
    }

    LOG_ERROR_HI("Can't find value for \"%s\"", key);

    return false;
}


int FileJSON::GetIntValue(pchar key1, pchar key2)
{
    CHECK_ON_VALID_INT;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        if (it->value.HasMember(key2))
        {
            it = it->value.FindMember(key2);

            if (it->value.IsInt())
            {
                return it->value.GetInt();
            }
        }
    }

    LOG_ERROR_HI("Can't find value for \"%s\" \"%s\"", key1, key2);

    return -1;
}


pchar FileJSON::GetStringValue(pchar key)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key);

    if (it != document->MemberEnd() && it->value.IsString())
    {
        return it->value.GetString();
    }

    LOG_ERROR_HI("Can't find value for \"%s\" key in file \"%s\"", key, full_path.c_str());

    return "";
}


pchar FileJSON::GetStringValue(pchar key1, pchar key2)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        if (it->value.HasMember(key2))
        {
            it = it->value.FindMember(key2);

            if (it->value.IsString())
            {
                return it->value.GetString();
            }
        }
    }

    LOG_ERROR_HI("Can't find value for \"%s\" \"%s\"", key1, key2);

    return "";
}


pchar FileJSON::GetStringValue(pchar key1, pchar key2, pchar key3)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document->FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        it = it->value.FindMember(key2);

        if (it->value.GetType() == rapidjson::Type::kObjectType)
        {
            if (it->value.HasMember(key3))
            {
                it = it->value.FindMember(key3);

                if (it->value.IsString())
                {
                    return it->value.GetString();
                }
            }
        }
    }

    LOG_ERROR_HI("Can't find value for \"%s\" \"%s\" \"%s\"", key1, key2, key3);

    return "";
}


bool FileJSON::GetVectorStrings(pchar key, Array<String<>> &strings)
{
    strings.ClearArray();

    if (!isValid)
    {
        LOG_ERROR_HI("Configuration file is not valid");
        return false;
    }

    if (document->HasMember(key))
    {
        rapidjson::Value::ConstMemberIterator it = document->FindMember(key);

        auto &value = it->value;

        if (value.IsArray() && value.Size() > 0)
        {
            for (rapidjson::Value::ConstValueIterator elem = value.Begin(); elem != value.End(); ++elem)
            {
                if (elem->IsString())
                {
                    strings.AppendArrayElement(elem->GetString());
                }
            }
        }
    }

    return strings.GetArrayElementCount() != 0;
}
