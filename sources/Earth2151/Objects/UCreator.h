// 2025/02/14 15:45:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Objects/Units/Units.h"
#include "Objects/Structures/Structures.h"


// Этот модуль создаёт разные вещи в основном потоке

namespace UCreator
{
    void Update();

    void NewTankForFactory(const Point2D &, StructureObject *);

    void NewRocket(const Point3D &, Model *);

    void DeleteWorldNode(Node *);
}
