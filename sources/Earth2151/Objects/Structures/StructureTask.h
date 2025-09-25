// 2025/02/12 16:24:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Task.h"
#include "Objects/Structures/Structures.h"
#include "Objects/Units/Units.h"


class StructureTask : public Task
{
public:

    StructureTask(StructureObject *_controller) :
        Task(), controller(_controller) { }

protected:

    StructureObject *controller = nullptr;
};


class ProduceUnitTask : public StructureTask
{
public:

    ProduceUnitTask(StructureObject *_controller, const Point3D &_move_to, pchar _name_type, int num) :
        StructureTask(_controller), name_type(_name_type), units_left(num), move_to(_move_to) { }

    virtual void Update() override;

    void SetLastUnit(GUnit *unit)
    {
        last_unit = unit;
        state = UnitIsCreated;
    }

private:

    enum State
    {
        Idle,               // Простаиваем
        Production,         // Изготавливаем подразделение
        WaitCreateUnit,     // Создано задание на создание юнита
        UnitIsCreated,      // Юнит создан
        OpenDoor,           // Открывается дверь
        RunUnit,            // Юнит выезжает
        CloseDoor           // Дверь закрывается

    } state = Idle;

    String<> name_type;

    int units_left = 0;             // Столько подразделений осталось произвести (включая производящийся в данный момент)

    uint time_finish_uinit = 0;     // В это время юнит должен быть изготовлен

    Point3D move_to;                // Сюда посылать подразделения после происзводства

    GUnit *last_unit = nullptr;    // Последний произведённый юнит
};
