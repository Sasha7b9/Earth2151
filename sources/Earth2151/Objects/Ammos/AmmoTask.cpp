// 2025/02/02 13:00:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Ammos/AmmoTask.h"
#include "Objects/Objects.h"
#include "Objects/Storage.h"


AmmoTask::AmmoTask(AmmoObject *_controller) : Task(),
    controller(_controller)
{

}


DestroyTargetTask::DestroyTargetTask(AmmoObject *_controller, Node *_node) : AmmoTask(_controller),
    target(_node)
{
}


void DestroyTargetTask::Update()
{
    if (SO::ExistNode(target))
    {
        /*
        *   1. Построим плоскость по двум векторам - направление движения и направление на цель.
        *   2. Будем вращаться вокруг вектора, перпендикулярного данной плоскости, чтобы навестись на цель
        */

        float dot = 0.0f;

        Vector3D cross = CalculateCrossDot(dot);

        controller->Rotate(cross, GetPositionTarget() - controller->GetTargetNode()->GetWorldPosition());

        prev_cross = cross;

        first_cross = false;

        controller->MoveForward(1.0f);

    }
    else
    {
        is_finished = true;
    }
}


Vector3D DestroyTargetTask::CalculateCrossDot(float &dot) const
{
    Point3D current_position = controller->GetTargetNode()->GetNodePosition();

    Vector3D dir_on_target = GetPositionTarget() - current_position;    // Направление на цель движения
//    dir_on_target.z = 0.0f;                                             // Нас интересует направление в проекции сверху, поэтому выключаем координату высоты
    dir_on_target.Normalize();

    Vector3D dir_forward = controller->GetDirectionForward();           // А в этом направлении движемся
//    dir_forward.z = 0.0f;
    dir_forward.Normalize();

    dot = Terathon::Dot(dir_on_target, dir_forward);

    static int counter = 0;

    if (String<>("Rocket") == controller->GetTargetNode()->GetNodeName())
    {
        counter++;
    }

    if (dot == 1.0f)
    {
        // Смотрим прямо на цель
    }
    else if (dot == -1.0f)
    {
        // Цель точно сзади
    }
    else if (dot > 0)
    {
        // Цель впереди
    }
    else
    {
        // Цельпозади
    }

    return Terathon::Cross(dir_on_target, dir_forward).Normalize();
}


Vector3D DestroyTargetTask::CalculateCross() const
{
    Point3D current_position = controller->GetTargetNode()->GetWorldPosition();

    Vector3D dir_on_target = GetPositionTarget() - current_position;    // Направление на цель движения
    dir_on_target.Normalize();

    Vector3D dir_forward = controller->GetDirectionForward();           // А в этом направлении движемся
    dir_forward.Normalize();

    return Terathon::Cross(dir_on_target, dir_forward);
}


Point3D DestroyTargetTask::GetPositionTarget() const
{
    if (SO::ExistNode(target))
    {
        return target->GetWorldPosition();
    }

    return Point3D::zero;
}


float DestroyTargetTask::CalculateDistance() const
{
    Vector3D vec = GetPositionTarget() - controller->GetTargetNode()->GetNodePosition();

    return Magnitude(vec);
}
