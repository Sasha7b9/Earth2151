// 2025/02/01 21:58:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Objects.h"


class Door;


// Базовый контроллер для зданий
class StructureObject : public StaticObject
{
public:

    StructureObject(pchar name_type) : StaticObject(name_type)
    {
        StaticObject::OnEventRebuild();
    }

    virtual void BeginOpenDoor() { }
    virtual void BeginCloseDoor() { }
    virtual bool DoorIsOpened() const { return false; }
    virtual bool DoorIsClosed() const { return true; }

protected:

    virtual void PreprocessController() override
    {
        StaticObject::PreprocessController();
    }

    virtual void Move() override
    {
        StaticObject::Move();
    }
};
