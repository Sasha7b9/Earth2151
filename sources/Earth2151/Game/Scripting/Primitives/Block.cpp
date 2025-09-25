// 2025/03/28 20:05:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Scripting/Primitives/Block.h"
#include "Game/Scripting/Objects/Object.h"
#include "Utils/Math.h"
#include <string>


Script::~Script()
{
    for (int i = 0; i < states.GetArrayElementCount(); i++)
    {
        delete states[i];
    }

    states.PurgeArray();

    for (int i = 0; i < events.GetArrayElementCount(); i++)
    {
        delete events[i];
    }

    events.PurgeArray();
}


State::~State()
{
    for (int i = 0; i < operators.GetArrayElementCount(); i++)
    {
        delete operators[i];
    }

    operators.PurgeArray();
}


OperatorIf::~OperatorIf()
{
    for (int num = 0; num < operators.GetArrayElementCount(); num++)
    {
        Array<Operator *> &elem = operators[num];

        for (int i = 0; i < elem.GetArrayElementCount(); i++)
        {
            delete elem[i];
        }
    }
}


bool Operator::Execute()
{
    LOG_ERROR_HI("Operator \"%s\"", type_operator.Name());

    return true;
}


bool OperatorIf::Execute()
{
    for (int i = 0; i < conditions.GetArrayElementCount(); i++)
    {
        if (Condition(conditions[i]))
        {
            RunOperators(operators[i]);
            return true;
        }
    }

    if (conditions.GetArrayElementCount() != operators.GetArrayElementCount())
    {
        RunOperators(operators[operators.GetArrayElementCount() - 1]);
    }

    return true;
}


bool OperatorIf::Condition(const String<> &condition) const
{
    Var *var = object->script->GetVar(condition);

    if (var)
    {
        return var->value != 0;
    }

    LOG_ERROR_HI("Not implemented");

    return false;
}


void OperatorIf::RunOperators(const Array<Operator *> &)
{
    LOG_ERROR_HI("Not implemented");
}


pchar TypeOperator::Name() const
{
    static const pchar names[Count] =
    {
        "If",
        "Call",
        "Return",
        "Assign",
        "Declaration"
    };

    return names[value];
}


pchar TypeBlock::Name() const
{
    static const pchar names[Count] =
    {
        "Script",
        "State",
        "Event",
        "Operator",
        "Command"
    };

    return names[value];
}


void Block::Log()
{
    tabs = 0;

    LOG_WRITE("Block : name \"%s\", type \"%s\"", name.c_str(), type_block.Name());

    tabs++;
}


void OperatorFunction::Log()
{
    char message[512];

    std::sprintf(message, "Call \"%s\",", name_func.c_str());

    for (int i = 0; i < parameters.GetArrayElementCount(); i++)
    {
        std::strcat(message, " ");
        std::strcat(message, parameters[i].c_str());
    }

    LOG_WRITE(message);
}


void OperatorDeclaration::Log()
{
    LOG_WRITE("Declaration %s = %d", var.name.c_str(), var.value);
}


void OperatorNull::Log()
{
    LOG_WRITE("Operator null");
}


void OperatorReturn::Log()
{
    LOG_WRITE("Return %s, %u", name_state.c_str(), time);
}


void OperatorAssign::Log()
{
    LOG_WRITE("Assign %s = %s", left.c_str(), right.c_str());
}


void Operator::Log()
{
    LOG_ERROR_HI("Not implemented for Operator \"%s\"", type_operator.Name());
}


void Script::Reset()
{
    current_state = FindState("Initialize");
}


bool Script::GetInt(pchar _name, int &value)
{
    Var *v = GetVar(_name);

    if (v)
    {
        value = v->value;
        return true;
    }

    return false;
}


Var *Script::GetVar(pchar _name)
{
    Var *v = current_state->GetVar(_name);

    if (v)
    {
        return v;
    }

    for (int i = 0; i < vars.GetArrayElementCount(); i++)
    {
        if (vars[i].name == _name)
        {
            return &vars[i];
        }
    }

    for (int i = 0; i < consts.GetArrayElementCount(); i++)
    {
        if (consts[i].name == _name)
        {
            return &consts[i];
        }
    }

    return nullptr;
}


State *Script::FindState(pchar name_state)
{
    for (State *state : states)
    {
        if (state->name == name_state)
        {
            return state;
        }
    }

    return nullptr;
}


void Script::SetCurrentState(pchar _name)
{
    current_state = FindState(_name);

    if (current_state)
    {
        LOG_WRITE("Set state \"%s\"", _name);
    }
    else
    {
        LOG_ERROR_HI("Can not set state \"%s\"", _name);
    }
}


void Script::Execute()
{
    current_state->Execute();

//    current_state->Log();
}


void State::Log()
{
    LOG_WRITE("                Log");
    LOG_WRITE("State \"%s\"", name.c_str());
    LOG_WRITE("Vars :");

    for (int i = 0; i < vars.GetArrayElementCount(); i++)
    {
        LOG_WRITE("    %s = %d", vars[i].name.c_str(), vars[i].value);
    }

    for (int i = 0; i < operators.GetArrayElementCount(); i++)
    {
        operators[i]->Log();
    }
}


void State::Execute()
{
//    LOG_WRITE("                 Execute %s", name.c_str());

    if (current_operator)
    {
        if (current_operator->Execute())
        {
            current_operator = nullptr;
        }
    }
    else
    {
        for (int i = 0; i < operators.GetArrayElementCount(); i++)
        {
            operators[i]->Execute();
        }
    }
}


bool OperatorDeclaration::Execute()
{
    if (state->GetVar(var.name))
    {
        state->GetVar(var.name)->value = var.value;
    }
    else
    {
        state->AppendVar(var);
    }

    return true;
}


bool OperatorNull::Execute()
{
    return true;
}


OperatorFunction::~OperatorFunction()
{
    SAFE_DELETE(result);
}


bool OperatorFunction::Execute()
{
    object->ExecuteOperatorFunction(this);

    return true;
}


bool OperatorReturn::Execute()
{
    if (state->current_operator != this)
    {
        counter.Start(time);                                // Заводим таймер, который сработает через заданное время
        state->current_operator = this;
        LOG_WRITE("Set current operator :");
        state->current_operator->Log();
    }
    else
    {
        if (counter.IsFinished())
        {
            state->current_operator = nullptr;
            object->script->SetCurrentState(name_state);
            LOG_WRITE("Return is finished");
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}


OperatorAssign::~OperatorAssign()
{
    SAFE_DELETE(func);
}


bool OperatorAssign::Execute()
{
    Var *v = object->script->GetVar(left);

    if (v)
    {
        bool result = true;

        Var *rhs = object->script->GetVar(right);

        if (right == "true")
        {
            v->value = 1;
        }
        else if (right == "false")
        {
            v->value = 0;
        }
        else if (rhs)
        {
            v->value = rhs->value;
        }
        else
        {
            result = false;
        }

        if (result)
        {
            LOG_WRITE("%s = %d", v->name.c_str(), v->value);
        }
        else
        {
            if (func)
            {
                func->Execute();
            }

            LOG_ERROR_MID("Can to find var \"%s\"", right.c_str());
        }
    }
    else
    {
        LOG_ERROR_MID("Can to find var \"%s\"", left.c_str());
    }

    return true;
}


Var *State::GetVar(pchar _name)
{
    for (Var &var : vars)
    {
        if (var.name == _name)
        {
            return &var;
        }
    }

    return nullptr;
}


void State::AppendVar(const Var &new_var)
{
    if (GetVar(new_var.name))
    {
        GetVar(new_var.name)->value = new_var.value;
    }

    vars.AppendArrayElement(new_var);
}


int OperatorFunction::ResolveParameterInt(int num_param)
{
    if (num_param >= parameters.GetArrayElementCount())
    {
        return 0;
    }

    String<> &name_param = parameters[num_param];

    Var *var_state = object->script->current_state->GetVar(name_param);     // Сначала пытаемся найти локальную переменную функции

    if (var_state)
    {
        return var_state->value;
    }

    int value = 0;

    if (object->script->GetInt(name_param, value))                          // А теперь - переменную скрипта по имени
    {
        return value;
    }

                                                                            // Если не получилось - пробуем извлечь значение
    std::size_t pos{};

    String<> &param = parameters[num_param];

    pchar v = param.c_str();

    if (param == "playerLocal")
    {
        LOG_WARNING_HI("Parameter Int = playerLocal");
        return 0;
    }
    else if (param == "playerAI")
    {
        LOG_WARNING_HI("Parameter Int = playerAI");
        return 1;
    }
    else if (param == "true")
    {
        LOG_WARNING_HI("Parameter Int = true");
        return 1;
    }
    else if (param == "false")
    {
        LOG_WARNING_HI("Parameter Int = false");
        return 0;
    }

    try
    {
        value = std::stoi(v, &pos);
    }
    catch (...)
    {
        LOG_ERROR_HI("Excecption for \"value = std::stoi(v, &pos)\" : v = %s, pos = %u", v, pos);

        return -99999999;
    }

    if (pos != std::strlen(v))                  // Преобразованы не все символы строки, т.е. это не значение, а математическое выражение
    {
        return M::ResolveMathExpression(v);
    }

    return value;
}


String<> &OperatorFunction::ResolveParameterStr(int num_param)
{
    static String<> null{ "" };

    if (num_param >= parameters.GetArrayElementCount())
    {
        return null;
    }

    String <> &res = parameters[num_param];

    if (res == "null")
    {
        return null;
    }

    return res;
}
