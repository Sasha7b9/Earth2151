// 2025/03/28 20:05:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/Primitives/Block.h"
#include "Earth2150/Scripting/Objects/SObject.h"
#include "Utils/Math.h"
#include <string>


S::Script::~Script()
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


S::State::~State()
{
    for (int i = 0; i < operators.GetArrayElementCount(); i++)
    {
        delete operators[i];
    }

    operators.PurgeArray();
}


S::OperatorIf::~OperatorIf()
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


bool S::Operator::Execute()
{
    LOG_ERROR("Operator \"%s\"", type_operator.Name());

    return true;
}


bool S::OperatorIf::Execute()
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


bool S::OperatorIf::Condition(const String<> &condition) const
{
    Var *var = object->script->GetVar(condition);

    if (var)
    {
        return var->value != 0;
    }

    LOG_ERROR("Not implemented");

    return false;
}


void S::OperatorIf::RunOperators(const Array<Operator *> &)
{
    LOG_ERROR("Not implemented");
}


pchar S::TypeOperator::Name() const
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


pchar S::TypeBlock::Name() const
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


void S::Block::Log()
{
    tabs = 0;

    LOG_WRITE("Block : name \"%s\", type \"%s\"", name.c_str(), type_block.Name());

    tabs++;
}


void S::OperatorCall::Log()
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


void S::OperatorDeclaration::Log()
{
    LOG_WRITE("Declaration %s = %d", var.name.c_str(), var.value);
}


void S::OperatorReturn::Log()
{
    LOG_WRITE("Return %s, %u", name_state.c_str(), time);
}


void S::OperatorAssign::Log()
{
    LOG_WRITE("Assign %s = %s", left.c_str(), right.c_str());
}


void S::Operator::Log()
{
    LOG_ERROR("Not implemented for Operator \"%s\"", type_operator.Name());
}


void S::Script::Reset()
{
    current_state = FindState("Initialize");
}


bool S::Script::GetInt(pchar name, int &value)
{
    Var *v = GetVar(name);

    if (v)
    {
        value = v->value;
        return true;
    }

    return false;
}


S::Var *S::Script::GetVar(pchar name)
{
    Var *v = current_state->GetVar(name);

    if (v)
    {
        return v;
    }

    for (int i = 0; i < vars.GetArrayElementCount(); i++)
    {
        if (vars[i].name == name)
        {
            return &vars[i];
        }
    }

    for (int i = 0; i < consts.GetArrayElementCount(); i++)
    {
        if (consts[i].name == name)
        {
            return &consts[i];
        }
    }

    return nullptr;
}


S::State *S::Script::FindState(pchar name_state)
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


void S::Script::SetCurrentState(pchar name)
{
    current_state = FindState(name);

    if (current_state)
    {
        LOG_WRITE("Set state \"%s\"", name);
    }
    else
    {
        LOG_ERROR("Can not set state \"%s\"", name);
    }
}


void S::Script::Execute()
{
    current_state->Execute();

//    current_state->Log();
}


void S::State::Log()
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


void S::State::Execute()
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


bool S::OperatorDeclaration::Execute()
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


bool S::OperatorCall::Execute()
{
    object->ExecuteOperatorCall(this);

    return true;
}


bool S::OperatorReturn::Execute()
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


bool S::OperatorAssign::Execute()
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
            LOG_ERROR("Can to find var \"%s\"", right.c_str());
        }
    }
    else
    {
        LOG_ERROR("Can to find var \"%s\"", left.c_str());
    }

    return true;
}


S::Var *S::State::GetVar(pchar name)
{
    for (Var &var : vars)
    {
        if (var.name == name)
        {
            return &var;
        }
    }

    return nullptr;
}


void S::State::AppendVar(const Var &new_var)
{
    if (GetVar(new_var.name))
    {
        GetVar(new_var.name)->value = new_var.value;
    }

    vars.AppendArrayElement(new_var);
}


int S::OperatorCall::ResolveParameterInt(int num_param)
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

    try
    {
        value = std::stoi(v, &pos);
    }
    catch (...)
    {
        if (param == "playerLocal")
        {
            return 0;
        }
        else if (param == "playerAI")
        {
            return 1;
        }
        else if (param == "true")
        {
            return 1;
        }
        else if (param == "false")
        {
            return 0;
        }

        return -99999999;
    }

    if (pos != std::strlen(v))                  // Преобразованы не все символы строки, т.е. это не значение, а математическое выражение
    {
        return M::ResolveMathExpression(v);
    }

    return value;
}


String<> &S::OperatorCall::ResolveParameterStr(int num_param)
{
    static String<> null{ "" };

    if (num_param >= parameters.GetArrayElementCount())
    {
        return null;
    }

    String <> &result = parameters[num_param];

    if (result == "null")
    {
        return null;
    }

    return parameters[num_param];
}
