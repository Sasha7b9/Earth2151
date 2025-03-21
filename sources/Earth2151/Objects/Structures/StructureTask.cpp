// 2025/02/12 16:24:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Structures/StructureTask.h"
#include "Objects/Units/UnitTask.h"
#include "Objects/UCreator.h"
#include "Clock.h"


void ProduceUnitTask::Update()
{
    if (state == Idle)
    {
        if (units_left)
        {
            state = Production;
            time_finish_uinit = UCOUNT_MS + 10000;
        }
    }
    else if(state == Production)
    {
        if (UCOUNT_MS >= time_finish_uinit)
        {
            units_left--;
            Point3D coord = controller->GetTargetNode()->GetWorldPosition();
            UCreator::NewTankForFactory({ coord.x + 1.0f, coord.y - 0.1f }, controller);
            state = WaitCreateUnit;
        }
    }
    else if (state == WaitCreateUnit)
    {
    }
    else if (state == UnitIsCreated)
    {
        last_unit->SetTask(new MoveTask(last_unit, { move_to.x, move_to.y }, nullptr));
        controller->BeginOpenDoor();
        state = OpenDoor;
    }
    else if (state == OpenDoor)
    {
        if (controller->DoorIsOpened())
        {
            last_unit->WakeController();
            state = RunUnit;
        }
    }
    else if (state == RunUnit)
    {
        Vector3D distance = controller->GetTargetNode()->GetWorldPosition() - last_unit->GetTargetNode()->GetWorldPosition();

        if (Magnitude(distance) >= 2.0f)
        {
            controller->BeginCloseDoor();
            state = CloseDoor;
        }
    }
    else if (state == CloseDoor)
    {
        if (controller->DoorIsClosed())
        {
            state = Idle;
        }
    }
}
