// 2025/02/02 13:00:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Task.h"
#include "Objects/Ammos/Ammos.h"


class AmmoTask : public Task
{
public:

    AmmoTask(AmmoObject *_controller);

protected:

    AmmoObject *controller = nullptr;

    // Возвращает true, если векторы смотрят в разных направлениях
    bool ChangedDirection(const Vector3D &, Vector3D &);
};


// Задание на уничтожение цели
class DestroyTargetTask : public AmmoTask
{
public:
    DestroyTargetTask(AmmoObject *, Node *target);

    virtual void Update() override;

private:

    bool first_cross = true;

    Node *target = nullptr;                     // За этим узлом охотимся

    Vector3D prev_cross = Vector3D::zero;       // Будем использовать для определения того, что при вращениии цель перешла слева направо или
                                                // наоборот

    Vector3D CalculateCross() const;            // Рассчитывает кросс-продукт между направлением на цель и направлением движения

    Vector3D CalculateCrossDot(float &) const;

    float CalculateDistance() const;            // Рассчитывает расстояние до цели

    Point3D GetPositionTarget() const;
};
