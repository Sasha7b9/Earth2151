// 2025/02/15 22:15:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Здесь находятся все созданные объекты и всё для работы с ними
// Storage objects
namespace SO
{
    // Возвращает модель в радиусе distance, но не requester
    Model *GetModelInSphere(Node *self, float distance);

    bool ExistNode(Node *);

    extern int ALL_ID;                      // Столько создано объектов
}
