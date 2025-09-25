// 2025/01/10 20:58:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Task.h"
#include "Objects/Units/Units.h"


// Элементарное задание для юнита. Например, переместиться из одной точки в другую


class UnitTask : public Task
{
public:

    UnitTask(GUnit *_controller, void (*_funcOnDestroy)()) : Task(),
        controller(_controller), funcOnDestroy(_funcOnDestroy) { }
    virtual ~UnitTask()
    {
        if (funcOnDestroy)
        {
            funcOnDestroy();
        }
    }

    virtual void Update() = 0;

protected:

    GUnit *controller = nullptr;

    void (*funcOnDestroy)();
};


class MoveTask : public UnitTask
{
public:

    MoveTask(GUnit *, const Point2D &to, void (*_funcOnDestroy)());
    MoveTask(GUnit *, pchar target_node_name, void (*_funcOnDestroy)());
    virtual ~MoveTask() override { }

    virtual void Update() override;

private:

    const pchar target_node_name = "";      // Если задано, то движемся к этому узлу

    const Point3D pos_target;               // Сюда движемся

    Vector3D prev_cross;                    // Будем использовать для определения того, что при вращениии цель перешла слева направо или
                                            // наоборот. При переходе cross.z меняет знак

    Vector3D CalculateCrossDot(float &dot) const;       // Рассчитывает кросс-продукт между направлением на цель и направлением движения

    float CalculateDistance() const;                    // Рассчитывает расстояние до цели

    Point3D GetPositionTarget() const;
};
