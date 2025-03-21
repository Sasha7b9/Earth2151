// 2025/01/13 15:42:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <scripthandle/scripthandle.h>
#include <string>


#define READ_FLOAT(var) var = ReadGlobalVar<float>(#var)
#define READ_STRING(var) var = ReadGlobalVar<std::string>(#var)
#define READ_VECTOR3(var) {std::array<float> array = ReadArray(#var); var.x = array[0]; var.y = array[1]; var.z = array[2]; }


class ControllerAS;


class ControllerAS : public Controller
{
public:

    pchar GetNameMeshFile() const { return name_mesh_file.c_str(); }

protected:

    // Передаётся имя типа юнита, например - "Airplane"
    // Он юнит будет считываться из файла "Airplane.as"
    ControllerAS(pchar name_type);

    virtual void PreprocessController() override;

    // В этой функции контроллера движка мы запускаем на выполнение в отдельный поток функцию нашего контроллера - Move()
    virtual void MoveController() override;

    // А это функция обновления нашего контроллера
    virtual void Move();

    // Вызывается после компиляции скрипта
    virtual void OnEventRebuild();

    template<class T>
    T ReadGlobalVar(pchar name);

    template<class T>
    std::array<T, 3> ReadArray(pchar name);

private:

    const pchar name_type;

    static asIScriptContext *context;
    static asIScriptEngine *engine;
    static asDWORD timeout;

    asIScriptModule *module = nullptr;

    // Возвращает значение функции name_func из скрипта
    float ExecuteFuncFV(pchar name_func);

    std::string ExecuteFuncSV(pchar name_func);

    String<> file_name;                                     // Полный путь к скрипту
    std::filesystem::file_time_type time_update_file;       // Время последнего изменения скрипта
    uint next_time_verify = 0;                              // Следующее время проверки файла а на изменение
    std::string name_mesh_file;

    static bool InitEngine();
    static void MessageCallback(const asSMessageInfo *, void *);
    static void ConfigureEngine();
    static void LineCallback(asIScriptContext *, asDWORD *timeOut);

    int CompileScript();

    // Перезагрузить скрипт, если требуется (изменился внешний .as)
    void ReloadIfNeed();

    // Используется для получения строки из скрипта
    static void GetString(const std::string &str) { global_string = str; }
    static std::string global_string;

    class ControllerJob : public BatchJob
    {
    public:

        ControllerJob(ExecuteCallback *execCallback, ControllerAS *controller) :
            BatchJob(execCallback, controller)
        {
        }
    };

    ControllerJob jobMove{ &JobMoveController, this };

    static void JobMoveController(Job *, void *);
};
