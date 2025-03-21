// 2025/02/14 15:50:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/UCreator.h"
#include "Objects/Ammos/AmmoTask.h"


namespace UCreator
{
    struct Struct
    {
        StructureObject *controller;
        Point2D coord;
    };

    static Array<Struct> tasks;

    struct StructRocket
    {
        Point3D coord;
        Model *target;
    };

    static Array<StructRocket> rockets;

    static Array<Node *> deletes;
}


void UCreator::Update()
{
    static Mutex mutex;

    mutex.Acquire();

    for (int i = 0; i < tasks.GetArrayElementCount(); i++)
    {
    }

    tasks.ClearArray();

    for (int i = 0; i < rockets.GetArrayElementCount(); i++)
    {
    }

    rockets.ClearArray();

    for (int i = 0; i < deletes.GetArrayElementCount(); i++)
    {
        delete deletes[i];
    }

    deletes.ClearArray();

    mutex.Release();
}


void UCreator::DeleteWorldNode(Node *node)
{
    static Mutex mutex;

    mutex.Acquire();

    for (int i = 0; i < deletes.GetArrayElementCount(); i++)
    {
        if (deletes[i] == node)
        {
            mutex.Release();

            return;
        }
    }

    deletes.AppendArrayElement(node);

    mutex.Release();
}


void UCreator::NewTankForFactory(const Point2D &coord, StructureObject *structure)
{
    static Mutex mutex;

    mutex.Acquire();

    tasks.AppendArrayElement(Struct{ structure, coord });

    mutex.Release();
}


void UCreator::NewRocket(const Point3D &coord, Model *target)
{
    static Mutex mutex;

    mutex.Acquire();

    rockets.AppendArrayElement(StructRocket{ coord, target });

    mutex.Release();
}
