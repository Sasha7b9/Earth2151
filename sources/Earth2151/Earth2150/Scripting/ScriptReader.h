// 2025/03/28 21:52:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Scripting/Primitives/Block.h"


namespace S
{
    class ScriptReader
    {
    public:
        ScriptReader(String<> &);

        String<> ReadString();

        String<> ReadString(pchar delim);

        // Считать строку в круглых скобках
        String<> ReadStringInParentheses();

        // Читает строку в кавычках
        String<> ReadStringQuotes();

        Script *CreateScript(SObject *);

        State *ReadState(SObject *);

        Event *ReadEvent(SObject *);

        void ReadConsts(Script *);

        void ReadEnums(Script *);

        void ReadFunction(SObject *, Script *);

        void ReadCommand(Script *);

        Operator *ReadOperator(SObject *, State *);

        OperatorIf *ReadOperatorIf(SObject *, State *);

        OperatorReturn *ReadOperatorReturn(SObject *, State *);

        OperatorCall *ReadOperatorCall(SObject *, State *, pchar name);

        OperatorDeclaration *ReadOperatorDeclaration(SObject *, State *);

        // Считывает строку, ограниченную ch1, ch2
        String<> ReadString(char ch1, char ch2);

        String<> ReadString(int start, int end);

        // Возвращает позицию одного из символов строки
        int FindSymbol(pchar);

    private:

        String<> content;

        char ReadSymbol();

        // "Возвращает" символ обратно, уменьшая r_index
        void PushSymbol();

        void SkipSeparators();

        void SkipSeparators(pchar);

        int r_index = 0;        // Это символ нужно читать

        int nesting_level = 0;  // Уровень вложенности. Когда встречается символ '{', он увеличивается на единицу, когда встречается '}', уменьшается не единицу.
                                // Когда он опять станет равным нулю, скрипт закончен.
    };
}
