// 2025/01/10 20:59:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Units/UnitTask.h"
#include "GameWorld.h"
#include "Objects/Units/Units.h"


MoveTask::MoveTask(Unit *_controller, const Point2D &to, void (*_funcOnDestroy)()) :
    UnitTask(_controller, _funcOnDestroy),
    pos_target(to)
{
    float dot = 0.0f;

    prev_cross = CalculateCrossDot(dot);
}


MoveTask::MoveTask(Unit *_controller, pchar _target_node_name, void (*_funcOnDestroy)()) :
    UnitTask(_controller, _funcOnDestroy),
    target_node_name(_target_node_name),
    pos_target(Point3D::zero)
{
    float dot = 0.0f;

    prev_cross = CalculateCrossDot(dot);
}


void MoveTask::Update()
{
    float dot = 0.0f;

    Vector3D cross = CalculateCrossDot(dot);

    if (Fsgn(cross.z) != Fsgn(prev_cross.z) &&      // Это условие означает, что мы едем прямо на цель
        CalculateDistance() < 1.0f)                 // Если до цели меньше 1
    {
        is_finished = true;
    }
    else
    {
        if (cross.z < 0.0f)
        {
            // Цель слева

            controller->_RotateAboutAxis(controller->GetDirectionUp(), 1.0f);
        }
        else
        {
            // Цель справа

            controller->_RotateAboutAxis(controller->GetDirectionUp(), -1.0f);
        }

        controller->MoveForward(Fsgn(dot));

        prev_cross = cross;
    }

    controller->GetTargetNode()->InvalidateNode();
}


Vector3D MoveTask::CalculateCrossDot(float &dot) const
{
    Point3D current_position = controller->GetTargetNode()->GetNodePosition();

    Vector3D dir_on_target = GetPositionTarget() - current_position;    // Направление на цель движения
    dir_on_target.z = 0.0f;                                             // Нас интересует направление в проекции сверху, поэтому выключаем координату высоты
    dir_on_target.Normalize();

    Vector3D dir_forward = controller->GetDirectionForward();           // А в этом направлении движемся
    dir_forward.z = 0.0f;
    dir_forward.Normalize();

    dot = Terathon::Dot(dir_on_target, dir_forward);

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

    return Terathon::Cross(dir_on_target, dir_forward);
}


float MoveTask::CalculateDistance() const
{
    Vector3D vec = GetPositionTarget() - controller->GetTargetNode()->GetNodePosition();

    return Magnitude(vec);
}


Point3D MoveTask::GetPositionTarget() const
{
    if (target_node_name[0])
    {
        Node *node = TheWorldMgr->GetWorld()->GetRootNode()->FindFirstNodeByName(target_node_name);

        return node ? node->GetWorldPosition() : Point3D::zero;
    }

    return pos_target;
}
