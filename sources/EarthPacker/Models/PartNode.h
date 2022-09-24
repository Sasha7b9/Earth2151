// 2022/09/24 21:48:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/ModelPart.h"


struct PartNode
{
    PartNode(int id, Models::ModelPart * = nullptr, PartNode *parent = nullptr);

    int id = -1;
    Models::ModelPart *part = nullptr;
    std::list<PartNode *> children;
    PartNode *parent;
};
