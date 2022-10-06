// 2022/09/24 21:48:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/PartNode.h"


PartNode::PartNode(int _id, ModelPart *_part, PartNode *_parent)
{
    id = _id;
    part = _part;
    parent = _parent;

    if (parent)
    {
        parent->children.push_back(this);
    }
}
