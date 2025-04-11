// 2025/03/28 20:05:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Clock.h"


namespace S
{
    struct State;
    struct Event;
    struct Operator;
    class  SObject;

    struct TypeBlock
    {
        enum E
        {
            Script,
            State,
            Event,
            Operator,
            Command,
            Count
        };

        E value;

        pchar Name() const;
    };


    struct TypeOperator
    {
        enum E
        {
            If,
            Call,
            Return,
            Assign,         // Оператор присваивания
            Declaration,    // int i;
            Count
        };

        E value;

        pchar Name() const;
    };


    struct Block
    {
        Block(TypeBlock::E t) : type_block{t} { }

        virtual ~Block() { }

        TypeBlock type_block;

        String<> name;

        virtual void Log();

        int tabs = 0;
    };

    struct Var
    {
        String<> name;
        int value;
    };

    struct Enum
    {
        String<> name;
        String<> value;
        bool multi;
    };

    struct Script : public Block
    {
        Script() : Block(TypeBlock::Script) { }

        virtual ~Script();

        Array<State *> states;
        Array<Event *> events;

        Array<Var> consts;
        Array<Var> vars;
        Array<Enum> enums;

        // Возвращает целочисленное значение из consts, vars, ежели таковое имеется
        bool GetInt(pchar name, int &value);

        Var *GetVar(pchar name);

        void Reset();

        void Execute();

        // Устанавливает в качестве текущего данное состояние
        void SetCurrentState(pchar);

        State *FindState(pchar);

        State *current_state = nullptr;
    };

    // state Initialize
    // {
    // }
    struct State : public Block
    {
        State() : Block(TypeBlock::State) { }

        virtual ~State();

        Array<Operator *>  operators;
        Array<Var>         vars;
        Operator          *current_operator = nullptr;      // Если это значение на равно нулю, то выполняется только этот оператор

        Var *GetVar(pchar);

        void AppendVar(const Var &);

        virtual void Log() override;

        void Execute();
    };


    struct Event : public State
    {
        Event() : State() { }

        String<> arguments;
    };


    struct Command : public Block
    {
        Command() : Block(TypeBlock::Command) { }

        String<> context;

        String<> arguments;
    };


    struct Function : public Event
    {
        Function() : Event() { }

        String<> type_return_value;
    };


    struct Operator : public Block
    {
        Operator(TypeOperator::E t, SObject *_object, State *_state) :
            Block(TypeBlock::Operator), type_operator{ t }, state{ _state }, object(_object) { }

        TypeOperator type_operator;

        State *state;

        SObject *object;

        // Если возвращает true, то выполнение оператора завершено
        virtual bool Execute();

        virtual void Log() override;
    };


    struct OperatorIf : public Operator
    {
        OperatorIf(SObject *_object, State *_state) : Operator(TypeOperator::If, _object, _state) { }

        virtual ~OperatorIf();

        Array<String<>>          conditions;     // По количеству if/elseif
        Array<Array<Operator *>> operators;      // По количеству if/elseif/else

        // true, если условие выполняется
        bool Condition(const String<> &) const;

        void RunOperators(const Array<Operator *> &);

        virtual bool Execute() override;
    };


    // LoadBase(1,baseUCS,1);
    struct OperatorCall : public Operator
    {
        // Функция вызывается для объекта object
        OperatorCall(SObject *_object, State *_state) : Operator(TypeOperator::Call, _object, _state) { }

        String<> name_func;

        Array<String<>> parameters;

        // По порядковому номеру в parameters возвращает его значение
        int ResolveParameterInt(int num);
        String<> &ResolveParameterStr(int);

        virtual void Log() override;

        virtual bool Execute() override;
    };


    struct OperatorReturn : public Operator
    {
        OperatorReturn(SObject *_object, State *_state) : Operator(TypeOperator::Return, _object, _state), time(0) { }

        uint time;                              // Через такое количество тиков переходим в новое состояние

        Counter counter;                        // По этому счётчику будем отсчитывать тики до завершения

        String<> name_state;                    // В это состояние будет переход

        virtual void Log() override;

        virtual bool Execute() override;
    };


    struct OperatorAssign : public Operator
    {
        OperatorAssign(SObject *_object, State *_state) : Operator(TypeOperator::Assign, _object, _state) { }

        String<> left;
        String<> right;

        virtual void Log() override;

        virtual bool Execute() override;
    };

    struct OperatorDeclaration : public Operator
    {
        OperatorDeclaration(SObject *_object, State *_state) : Operator(TypeOperator::Declaration, _object, _state) { }

        Var var;

        virtual void Log() override;

        virtual bool Execute() override;
    };
}
