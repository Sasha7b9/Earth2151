// 2025/02/01 19:35:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Units/Units.h"


void Unit::OnEventRebuild()
{
    MovedObject::OnEventRebuild();
}


GroundUnitObject::GroundUnitObject(pchar name_type) : Unit(name_type)
{
}


void GroundUnitObject::OnEventRebuild()
{
    Unit::OnEventRebuild();
}


AirUnitObject::AirUnitObject(pchar name_type) : Unit(name_type)
{
    AirUnitObject::OnEventRebuild();
}


void AirUnitObject::OnEventRebuild()
{
    Unit::OnEventRebuild();

    READ_FLOAT(cruising_altitude);
}
