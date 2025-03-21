// 2025/02/01 19:36:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Objects.h"


// Базовый класс для юнитов
class Unit : public MovedObject
{
protected:

    Unit(pchar name_type) : MovedObject(name_type)
    {
        MovedObject::OnEventRebuild();
    }

    virtual void OnEventRebuild() override;
};


// Базовый контроллер для наземных юнитов
class GroundUnitObject : public Unit
{
public:

    GroundUnitObject(pchar name_type);

    virtual void OnEventRebuild() override;

private:
};


// Базовый контроллер для воздушных юнитов
class AirUnitObject : public Unit
{
public:

    AirUnitObject(pchar name_type);

    virtual void OnEventRebuild() override;

    float GetCruisingAltitude() const { return cruising_altitude; }         // Типовая высота нахождения над поверхностью

    float cruising_altitude = 0.0f;
};

