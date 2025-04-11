// 2025/03/28 21:52:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Scripting/ScriptReader.h"
#include "Utils/StringUtils.h"



S::ScriptReader::ScriptReader(String<> &_content_) : content(_content_)
{
}


String<> S::ScriptReader::ReadString()
{
    SkipSeparators();

    String<> result{ "" };

    char symbol = ReadSymbol();

    while (symbol != ' ' && symbol != '{' && symbol != ';')
    {
        char buffer[2] = { symbol, '\0' };

        result.AppendString(buffer);

        symbol = ReadSymbol();
    }

    PushSymbol();

    return result;
}


String<> S::ScriptReader::ReadString(int start, int end)
{
    String<> result;

    int pointer = start;

    while (pointer < end)
    {
        char buffer[2] = { content[pointer], '\0' };

        result.AppendString(buffer);

        pointer++;
    }

    return result;
}


String<> S::ScriptReader::ReadString(char ch1, char ch2)
{
    char symbol = ReadSymbol();

    while (symbol != ch1)
    {
        symbol = ReadSymbol();
    }

    symbol = ReadSymbol();

    if (symbol == ch2)              // Между ch1 и сh2 ничего нету
    {
        LOG_WRITE("reading stirng \"\"");

        return "";
    }

    String<> result;

    int level = 1;

    while (level != 0)
    {
        char buffer[2] = { symbol, '\0' };
        result.AppendString(buffer);
        symbol = ReadSymbol();

        if (symbol == ch1)
        {
            level++;
        }
        else if (symbol == ch2)
        {
            level--;
        }
    }

    return result;
}


String<> S::ScriptReader::ReadStringInParentheses()
{
    SkipSeparators(" ");

    char symbol = ReadSymbol();

//    pchar pointer = content.c_str() + r_index;

    if (symbol != '(')
    {
        LOG_ERROR("Bad symbol. Not (");

        return "";
    }

    int level = 1;

    String<> result;

    while (level > 0)
    {
        symbol = ReadSymbol();

        if (symbol == ')')
        {
            level--;
        }
        if (symbol == '(')
        {
            level++;
        }

        if (level > 0)
        {
            char buffer[2] = { symbol, '\0' };

            result.AppendString(buffer);
        }
    }

    return result;
}


String<> S::ScriptReader::ReadString(pchar delim)
{
    SkipSeparators(delim);

    pchar pointer = content.c_str() + r_index;

    if (*pointer == '\0')
    {
        return "";
    }

    String<> result{ "" };

    char symbol = ReadSymbol();

    bool flag = true;

    int num_append = 0;

    size_t len = std::strlen(delim);

    while (flag)
    {
        for (uint i = 0; i < len; i++)
        {
            if (symbol == delim[i])
            {
                flag = false;
            }
        }

        if (flag)
        {
            char buffer[2] = { symbol, '\0' };
            result.AppendString(buffer);
            num_append++;
        }

        symbol = ReadSymbol();
    }

    pointer = content.c_str() + r_index;

    PushSymbol();

    PushSymbol();

    if (num_append == 0)
    {
        ReadSymbol();

        return "";
    }

    return result;
}


String<> S::ScriptReader::ReadStringQuotes()
{
    String<> result = "";

    SkipSeparators(" \"");

    char symbol = ReadSymbol();

    while (symbol != '\"')
    {
        char buffer[2] = { symbol, '\0' };

        result.AppendString(buffer);

        symbol = ReadSymbol();
    }

    return result;
}


void S::ScriptReader::SkipSeparators()
{
    while (content[r_index] == ' ')
    {
        r_index++;
    }
}


void S::ScriptReader::SkipSeparators(pchar sep)
{
    bool flag = true;

    size_t len = std::strlen(sep);

    while (flag)
    {
        flag = false;

        char symbol = ReadSymbol();

        for (uint i = 0; i < len; i++)
        {
            if (symbol == sep[i])
            {
                flag = true;
            }
        }
    }

    PushSymbol();
}


S::OperatorDeclaration *S::ScriptReader::ReadOperatorDeclaration(SObject *object, State *state)
{
    OperatorDeclaration *oper = new OperatorDeclaration(object, state);

    SkipSeparators(" ");

    String<> word = ReadString(";");

    SkipSeparators(" ;");

    oper->var.name = word;
    oper->var.value = 0;

    return oper;
}


S::OperatorIf *S::ScriptReader::ReadOperatorIf(SObject *object, State *state)
{
    OperatorIf *oper = new OperatorIf(object, state);

    {
        // Здесь мы добавили первый блок

        String<> condition = ReadStringInParentheses();

        oper->conditions.AppendArrayElement(condition);

        int enter_level = nesting_level;

        SkipSeparators(" ");

        Array<Operator *>operators;

        if (ReadSymbol() == '{')                            // Начинается составной оператор
        {
            while (enter_level < nesting_level)
            {
                operators.AppendArrayElement(ReadOperator(object, state));
                SkipSeparators("; }");
            }
        }
        else                                                // Одинарный оператор
        {
            PushSymbol();
            operators.AppendArrayElement(ReadOperator(object, state));
        }

        oper->operators.AppendArrayElement(operators);
    }

    // А сейчас надо добавить все оставшиеся блоки

    while (true)
    {
        int pos = r_index;
        int level = nesting_level;

        SkipSeparators(" ;");

        String<> word = ReadString(" ");

        if (word == "else")
        {
            SkipSeparators(" ");

            word = ReadString(" (");

            if (word == "if")
            {
                String<> condition = ReadStringInParentheses();

                oper->conditions.AppendArrayElement(condition);
            }
            else
            {
                PushSymbol();
            }

            int enter_level = nesting_level;

            SkipSeparators(" ");

            Array<Operator *> operators;

            if (ReadSymbol() == '{')                            // Начинается составной оператор
            {
                while (enter_level < nesting_level)
                {
                    operators.AppendArrayElement(ReadOperator(object, state));
                    SkipSeparators("; }");
                }
            }
            else                                                // Одинарный оператор
            {
                PushSymbol();
                operators.AppendArrayElement(ReadOperator(object, state));
            }

            oper->operators.AppendArrayElement(operators);
        }
        else
        {
            r_index = pos;
            nesting_level = level;

            break;
        }
    }

    return oper;
}


S::OperatorCall *S::ScriptReader::ReadOperatorCall(SObject *object, State *state, pchar name)
{
    OperatorCall *oper = new OperatorCall(object, state);

    oper->name_func = name;

    SkipSeparators("(");

    String<> parameters = ReadString(";");

    parameters[parameters.GetStringLength() - 1] = '\0';

    SU::SplitToWords(parameters, oper->parameters, " ,");

    return oper;
}


S::OperatorReturn *S::ScriptReader::ReadOperatorReturn(SObject *object, State *state)
{
    OperatorReturn *oper = new OperatorReturn(object, state);

    int pos = FindSymbol(",;");

    if (content[pos] == ',')
    {
        oper->name_state = ReadString(" ,");

        String<> time = ReadString(", ;");

        oper->time = (uint)std::atoi(time.c_str());
    }
    else
    {
        oper->name_state = ReadString(";");

        oper->time = 0;
    }

    SkipSeparators(" ;}");

    return oper;
}


S::Operator *S::ScriptReader::ReadOperator(SObject *object, State *state)
{
    String<> word = ReadString(" (;}=");

    if (word == "int")
    {
        return ReadOperatorDeclaration(object, state);
    }
    else if (word == "if")
    {
        return ReadOperatorIf(object, state);
    }
    else if (word == "return")
    {
        return ReadOperatorReturn(object, state);
    }
    else if (word == "")                                    // Тоже return
    {
        OperatorReturn *oper = new OperatorReturn(object, state);

        oper->name_state = "";
        oper->time = 0;

        return oper;
    }
    else if (word == "true" || word == "false")
    {
        OperatorReturn *oper = new OperatorReturn(object, state);
        oper->name_state = word;
        oper->time = 0;
        return oper;
    }
    else
    {
        int pos_comma = FindSymbol(";");
        int pos_eq = FindSymbol("=");
        int pos_bracket = FindSymbol("}");

        if (pos_bracket > pos_eq && pos_bracket > pos_comma)
        {
            if (pos_eq > 0 && pos_eq < pos_comma)
            {
                OperatorAssign *ass = new OperatorAssign(object, state);

                ass->left = word;

                SkipSeparators(" =");

                ass->right = ReadString(" =;");

                SkipSeparators("; }");

                return ass;
            }
        }
    }

    return ReadOperatorCall(object, state, word);
}


S::State *S::ScriptReader::ReadState(SObject *object)
{
    int enter_nesting = nesting_level;

    while (ReadSymbol() != '{')
    {
    }

    State *state = new State();

    while (enter_nesting < nesting_level)
    {
        pchar pointer = content.c_str() + r_index;

        if (*pointer == '}')                                        // Закончена функция. Опреатор выхода.
        {
            ReadSymbol();

            OperatorReturn *oper = new OperatorReturn(object, state);
            oper->name_state = "";
            oper->time = 0;

            state->operators.AppendArrayElement(oper);

            return state;
        }

        state->operators.AppendArrayElement(ReadOperator(object, state));

        SkipSeparators(";} ");

        pointer = content.c_str() + r_index;
    }

    return state;
}


void S::ScriptReader::ReadCommand(Script *script)
{
    Command *block = new Command();

    block->name = ReadString(" (");

    block->arguments = ReadStringInParentheses();

    int pos = r_index;

    int level = nesting_level;

    while (ReadSymbol() != '{')
    {
    }

    while (level != nesting_level)
    {
        ReadSymbol();
    }

    block->context = ReadString(pos, r_index);

    delete block;
}


void S::ScriptReader::ReadFunction(SObject *object, Script *script)
{
    Function *function = new Function();

    function->type_return_value = ReadString(" ");

    function->name = ReadString(" (");

    function->arguments = ReadStringInParentheses();

    int enter_nesting = nesting_level;

    while (ReadSymbol() != '{')
    {
    }

    pchar pointer = content.c_str() + r_index;

    while (enter_nesting < nesting_level)
    {
        SkipSeparators("; ");

        if (*pointer == '}')                                        // Закончена функция. Опреатор выхода.
        {
            ReadSymbol();

            OperatorReturn *oper = new OperatorReturn(object, function);
            oper->name_state = "";
            oper->time = 0;

            function->operators.AppendArrayElement(oper);

            break;
        }

        function->operators.AppendArrayElement(ReadOperator(object, function));

        SkipSeparators(" ;");
    }

    pointer = content.c_str() + r_index;

    script->states.AppendArrayElement(function);
}


S::Event *S::ScriptReader::ReadEvent(SObject *object)
{
    int enter_nesting = nesting_level;

    while (ReadSymbol() != '(')
    {
    }

    PushSymbol();

    Event *event = new Event();

    event->arguments = ReadStringInParentheses();

    SkipSeparators(" ){");

    while (enter_nesting < nesting_level)
    {
        SkipSeparators("; ");

        pchar pointer = content.c_str() + r_index;

        if (*pointer == '}')                                        // Закончена функция. Опреатор выхода.
        {
            ReadSymbol();

            OperatorReturn *oper = new OperatorReturn(object, event);
            oper->name_state = "";
            oper->time = 0;

            event->operators.AppendArrayElement(oper);

            return event;
        }

        event->operators.AppendArrayElement(ReadOperator(object, event));
    }

    return event;
}


void S::ScriptReader::ReadConsts(Script *script)
{
    int level = nesting_level;

    SkipSeparators(" {");

    while (level != nesting_level)
    {
        String<> name = ReadString("= ");
        SkipSeparators(" =");
        String<> value = ReadString("; ");

        Var var;
        var.name = name;
        var.value = std::atoi(value.c_str());

        script->consts.AppendArrayElement(var);

        SkipSeparators(" ;}");
    }
}


void S::ScriptReader::ReadEnums(Script *script)
{
    int level = nesting_level;

    String<> name = ReadString(" ");

    SkipSeparators(" {");

    bool multi = false;

    while (level != nesting_level)
    {
        String<> value = ReadString(" ,\"}");

        Enum en;
        en.name = name;
        en.value = value;
        en.multi = multi;

        if (value == "multi:")
        {
            multi = true;
        }
        else
        {
            script->enums.AppendArrayElement(en);
        }

        SkipSeparators(" },\"");
    }
}


S::Script *S::ScriptReader::CreateScript(SObject *object)
{
    int enter_nesting = nesting_level;

    while (ReadSymbol() != '{')                 // Находим открывающую скобку
    {
    }

    Script *script = new Script();

    while (enter_nesting < nesting_level)      // Пока не нашли закрывающую скобку
    {
        String<> word = ReadString(" ({};");

        if (word == "state")
        {
            int pos = FindSymbol(";{");

            if (content[pos] == ';')
            {
                r_index = pos + 1;
            }
            else
            {
                String<> state_name = ReadString();

                State *state = ReadState(object);

                state->name = state_name;

                script->states.AppendArrayElement(state);
            }
        }
        else if (word == "event")
        {
            int pos = FindSymbol(";{");

            if (content[pos] == ';')
            {
                r_index = pos + 1;
            }
            else
            {
                String<> state_name = ReadString(" (");

                Event *event = ReadEvent(object);

                event->name = state_name;

                script->events.AppendArrayElement(event);
            }
        }
        else if (word == "function")
        {
            ReadFunction(object, script);
        }
        else if (word == "consts")
        {
            ReadConsts(script);
        }
        else if (word == "command")
        {
            ReadCommand(script);
        }
        else if (word == "enum")
        {
            ReadEnums(script);
        }
        else if (word == "int")
        {
            Var var;

            var.name = ReadString(" ;");
            var.value = 0;

            script->vars.AppendArrayElement(var);
        }
        else if (word == "player")
        {
            Var var;
            var.name = ReadString(" ;");
            var.value = 0;

            script->vars.AppendArrayElement(var);
        }
        else if (word == "unitex")
        {
            Var var;
            var.name = ReadString(" ;");
            var.value = 0;

            script->vars.AppendArrayElement(var);
        }
        else if (word == "unit")
        {
            Var var;
            var.name = ReadString(" ;");
            var.value = 0;

            script->vars.AppendArrayElement(var);
        }
        else
        {
            LOG_ERROR("Unknown word %s", word.c_str());
        }

        SkipSeparators("} ");
    }

    return script;
}


char S::ScriptReader::ReadSymbol()
{
    char symbol = content[r_index++];

    if (symbol == '{')
    {
        nesting_level++;
    }
    else if (symbol == '}')
    {
        nesting_level--;
    }

    return symbol;
}


void S::ScriptReader::PushSymbol()
{
    r_index--;

    if (content[r_index] == '{')
    {
        nesting_level--;
    }
    else if (content[r_index] == '}')
    {
        nesting_level++;
    }
}


int S::ScriptReader::FindSymbol(pchar symbols)
{
    int index = r_index;

    size_t len = std::strlen(symbols);

    while (index < content.GetStringLength())
    {
        for (uint i = 0; i < len; i++)
        {
            if (content[index] == symbols[i])
            {
                return index;
            }
        }

        index++;
    }

    return -1;
}
