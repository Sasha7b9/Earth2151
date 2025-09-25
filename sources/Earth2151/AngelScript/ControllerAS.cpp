// 2025/01/13 15:42:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "AngelScript/ControllerAS.h"
#include "Earth2151.h"
#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <weakref/weakref.h>
#include <assert.h>
#include <type_traits>


template float ControllerAS::ReadGlobalVar(pchar name);


#ifdef WIN32
#else

#include <sys/time.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define UINT unsigned int 
typedef unsigned int DWORD;

DWORD timeGetTime()
{
    timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

#endif


std::string ControllerAS::global_string;


asIScriptEngine *ControllerAS::engine = nullptr;
asIScriptContext *ControllerAS::context = nullptr;
asDWORD ControllerAS::timeout = 0;


ControllerAS::ControllerAS(pchar _name_type) :
    Controller(Text::Hash(_name_type)),
    name_type(_name_type),
    file_name(Earth2151::DataFile(String<>(name_type) + ".as").c_str())
{
    static Mutex mutex;

    mutex.Acquire();

    if (engine == nullptr)
    {
        ControllerAS::InitEngine();
    }

    int r = ControllerAS::CompileScript(); assert(r >= 0);

    mutex.Release();
}


void ControllerAS::MoveController()
{
    Controller::MoveController();

    GetTargetNode()->InvalidateNode();

    TheWorldMgr->GetWorld()->SubmitWorldJob(&jobMove);
}


void ControllerAS::JobMoveController(Job *job, void *cookie)
{
    ControllerAS *controller = (ControllerAS *)cookie;

    controller->Move();

    controller->GetTargetNode()->InvalidateNode();
}


void ControllerAS::PreprocessController()
{
    Controller::PreprocessController();
}


void ControllerAS::Move()
{
//    static Mutex mutex;

//    mutex.Acquire();

//    ReloadIfNeed();

//    mutex.Release();
}


bool ControllerAS::InitEngine()
{
    engine = asCreateScriptEngine();

    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert(r >= 0);

    // Настраиваем скриптовый движок со всеми функциями, 
    // и переменные, которые должен иметь возможность использовать скрипт.
    ConfigureEngine();

    // Создаем контекст, который будет выполнять скрипт.
    context = engine->CreateContext();

    if (context == nullptr)
    {
        engine->Release();
        return false;
    }

    // Мы не хотим, чтобы скрипт зависал над приложением, например. с
    // бесконечный цикл, поэтому мы будем использовать функцию обратного вызова строки, чтобы установить таймаут
    // это прервет выполнение скрипта через определенное время. Прежде чем выполнить
    // скрипт, переменная timeOut будет установлена на время, когда скрипт должен прекратить выполнение.
    r = context->SetLineCallback(asFUNCTION(LineCallback), &timeout, asCALL_CDECL);
    if (r < 0)
    {
        context->Release();
        engine->Release();
        return false;
    }

    return true;
}


void ControllerAS::MessageCallback(const asSMessageInfo *msg, void *)
{
    pchar type = "ERR ";
    if (msg->type == asMSGTYPE_WARNING)
    {
        type = "WARN";
    }
    else if (msg->type == asMSGTYPE_INFORMATION)
    {
        type = "INFO";
    }

    LOG_WRITE("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
}


void ControllerAS::ConfigureEngine()
{
    //    int r = 0;

    RegisterStdString(engine);

    engine->RegisterGlobalFunction("void ReturnString(const string &in)", asFUNCTION(GetString), asCALL_CDECL);

    // Регистрируем тип строки скрипта
    // Посмотрите на реализацию этой функции для получения дополнительной информации  
    // о том, как зарегистрировать собственный тип строки и другие типы объектов.
    if (!strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
    {
        // Зарегистрируем функции, которые скриптам будет разрешено использовать.
        // Обратите внимание, как код возврата проверяется с помощью Assert(). Это помогает
        // мы обнаруживаем, где возникает проблема, и не загрязняем код
        // с большим количеством if. Если ошибка возникает в режиме выпуска, она будет
        // быть пойманным при сборке скрипта, поэтому в этом нет необходимости
        // чтобы провести проверку и здесь.
//        r = engine->RegisterGlobalFunction("float GetMaxSpeed()", asFUNCTION(PrintString), asCALL_CDECL); assert(r >= 0);
    }
    else
    {
        // Обратите внимание, что регистрация практически идентична описанной выше.
//        r = engine->RegisterGlobalFunction("float GetMaxSpeed(string &in)", asFUNCTION(PrintString_Generic), asCALL_GENERIC); assert(r >= 0);
    }

    // Можно зарегистрировать функции, свойства и типы в 
    // группы конфигурации. При компиляции скриптов это тогда
    // определить, какие группы конфигурации должны быть доступны для этого
    // скрипт. При необходимости группу конфигурации также можно удалить из
    // движок, чтобы можно было изменить конфигурацию движка 
    // без необходимости перекомпилировать все скрипты.
}


void ControllerAS::LineCallback(asIScriptContext *_context, asDWORD *timeOut)
{
    // Если время ожидания истекло, мы прерываем выполнение скрипта.
    if (*timeOut < timeGetTime())
    {
        _context->Abort();
    }

    // Также можно было бы только приостановить выполнение скрипта,
    // вместо того, чтобы прерывать его. Это позволит приложению
    // чтобы возобновить выполнение с того места, на котором оно остановилось позже 
    // время, просто снова вызвав Execute().
}


int ControllerAS::CompileScript()
{
    time_update_file = std::filesystem::last_write_time(file_name.c_str());

    // Скрипт мы загрузим из файла на диске.
    FILE *f = fopen(file_name.c_str(), "rb");
    if (f == 0)
    {
        LOG_ERROR_HI("Failed to open the script file \"%s\".", file_name.c_str())
            return -1;
    }

    // Определить размер файла
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Вместо Win32 можно сделать следующее
    // int len = _filelength(_fileno(f));
    // Прочитать весь файл
    std::string script;
    script.resize(len);
    size_t c = fread(&script[0], len, 1, f);
    fclose(f);

    if (c == 0)
    {
        LOG_ERROR_HI("Failed to load script file.");
        return -1;
    }

    // Добавляем разделы скрипта, которые будут скомпилированы в исполняемый код.
    // Если мы хотим объединить более одного файла в один скрипт, то 
    // мы можем вызвать AddScriptSection() несколько раз для одного и того же модуля и
    // механизм сценариев будет относиться к ним всем, как к одному. Сценарий
    // имя раздела, позволит нам локализовать любые ошибки в коде скрипта.
    module = engine->GetModule(file_name.c_str(), asGM_ALWAYS_CREATE);
    int r = module->AddScriptSection("script", &script[0], len);
    if (r < 0)
    {
        LOG_ERROR_HI("AddScriptSection() failed");
        return -1;
    }

    // Компилируем скрипт. Если есть какие-либо сообщения компилятора, они будут
    // быть записано в поток сообщений, который мы установили сразу после создания 
    // скриптовый движок. Если нет ошибок и предупреждений, ничего не произойдет.
    // быть записано в поток.
    r = module->Build();
    if (r < 0)
    {
        LOG_ERROR_HI("Build() failed");
        return -1;
    }

    // Движок не сохраняет копии разделов скрипта после того, как Build() выполнила
    // вернулся. Так что если скрипт нужно перекомпилировать, то весь скрипт
    // разделы необходимо добавить заново.

    // Если мы хотим, чтобы несколько скриптов выполнялись в разное время, но 
    // которые не имеют прямого отношения друг к другу, то мы можем их скомпилировать
    // в отдельные модули скрипта. Каждый модуль использует свое собственное пространство имен и 
    // область видимости, поэтому имена функций и глобальные переменные не будут конфликтовать с
    // друг друга.

    OnEventRebuild();

    return 0;
}


void ControllerAS::ReloadIfNeed()
{
    if (TheTimeMgr->GetAbsoluteTime() < next_time_verify)
    {
        return;
    }

    next_time_verify = TheTimeMgr->GetAbsoluteTime() + 1000;

    if (std::filesystem::last_write_time(file_name.c_str()) != time_update_file)
    {
        CompileScript();
    }
}


template<class T>
T ControllerAS::ReadGlobalVar(pchar name)
{
    int index = module->GetGlobalVarIndexByName(name);
    T *var = (T *)module->GetAddressOfGlobalVar(index);

    if (var)
    {
        return *var;
    }

    LOG_ERROR_HI("Can not read var \"%s\" from \"%s.as\"", name, name_type);

    if (std::is_same<T, std::string>::value)
    {
        return T{};
    }

    return T{0};
}


float ControllerAS::ExecuteFuncFV(pchar name_func)
{
    float result = 0.0f;

    asIScriptFunction *funcGetMaxSpeed = module->GetFunctionByDecl(name_func);

    if (funcGetMaxSpeed == nullptr)
    {
        LOG_ERROR_HI("Function \"%s\" was not found", name_func);
        return result;
    }

    // Подготавливаем контекст скрипта с функцией, которую мы хотим выполнить. Подготовить()
    // необходимо вызывать в контексте перед каждой новой функцией скрипта, которая будет
    // выполнено. Обратите внимание: если вы собираетесь выполнить одну и ту же функцию несколько 
    // раз может быть хорошей идеей сохранить функцию, возвращаемую 
    // GetFunctionByDecl(), чтобы можно было пропустить этот относительно медленный вызов.
    int r = context->Prepare(funcGetMaxSpeed); assert(r >= 0);

    if (r < 0)
    {
        LOG_ERROR_HI("Failed to prepare the context");
        return result;
    }

    timeout = timeGetTime() + 1000;

    r = context->Execute(); assert(r >= 0);

    if (r != asEXECUTION_FINISHED)
    {
        // Казнь завершилась не так, как мы планировали. Определите, почему.
        if (r == asEXECUTION_ABORTED)
        {
            LOG_ERROR_HI("The script was aborted before it could finish. Probably it timed out.");
        }
        else if (r == asEXECUTION_EXCEPTION)
        {
            LOG_ERROR_HI("The script ended with an exception.");

            asIScriptFunction *fn = context->GetExceptionFunction();

            LOG_WRITE("     func: %s", fn->GetDeclaration());
            LOG_WRITE("     modl: %s", fn->GetModuleName());
            LOG_WRITE("     sect: %s", fn->GetScriptSectionName());
            LOG_WRITE("     line: %s", context->GetExceptionLineNumber());
            LOG_WRITE("     desc: %s", context->GetExceptionString());
        }
        else
        {
            LOG_ERROR_HI("The script ended for some unforeseen reason %s.", r);
        }
    }
    else
    {
        result = context->GetReturnFloat();
    }

    r = context->Unprepare(); assert(r >= 0);

    return result;
}


std::string ControllerAS::ExecuteFuncSV(pchar name_func)
{
    std::string result;

    asIScriptFunction *func = module->GetFunctionByDecl(name_func);

    if (func == nullptr)
    {
        LOG_ERROR_HI("Function \"%s\" was not found", name_func);
        return result;
    }

    // Подготавливаем контекст скрипта с функцией, которую мы хотим выполнить. Подготовить()
    // необходимо вызывать в контексте перед каждой новой функцией скрипта, которая будет
    // выполнено. Обратите внимание: если вы собираетесь выполнить одну и ту же функцию несколько 
    // раз может быть хорошей идеей сохранить функцию, возвращаемую 
    // GetFunctionByDecl(), чтобы можно было пропустить этот относительно медленный вызов.
    int r = context->Prepare(func); assert(r >= 0);

    if (r < 0)
    {
        LOG_ERROR_HI("Failed to prepare the context");
        return result;
    }

    timeout = timeGetTime() + 1000;

    r = context->Execute(); assert(r >= 0);

    if (r != asEXECUTION_FINISHED)
    {
        // Казнь завершилась не так, как мы планировали. Определите, почему.
        if (r == asEXECUTION_ABORTED)
        {
            LOG_ERROR_HI("The script was aborted before it could finish. Probably it timed out.");
        }
        else if (r == asEXECUTION_EXCEPTION)
        {
            LOG_ERROR_HI("The script ended with an exception.");

            asIScriptFunction *fn = context->GetExceptionFunction();

            LOG_WRITE("     func: %s", fn->GetDeclaration());
            LOG_WRITE("     modl: %s", fn->GetModuleName());
            LOG_WRITE("     sect: %s", fn->GetScriptSectionName());
            LOG_WRITE("     line: %s", context->GetExceptionLineNumber());
            LOG_WRITE("     desc: %s", context->GetExceptionString());
        }
        else
        {
            LOG_ERROR_HI("The script ended for some unforeseen reason %s.", r);
        }
    }
    else
    {
        result = global_string;
    }

    r = context->Unprepare(); assert(r >= 0);

    return result;
}


void ControllerAS::OnEventRebuild()
{
    READ_STRING(name_mesh_file);
}
