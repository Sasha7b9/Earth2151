// 2025/02/15 22:15:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Storage.h"


int SO::ALL_ID = 0;


Model *SO::GetModelInSphere(Node *self, float distance)
{
    auto Traverse = [&](Node *root) -> Node *
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        Node *child = root->GetFirstSubnode();

        while (child)
        {
            if (child != self &&
                Magnitude(self->GetWorldPosition() - child->GetWorldPosition()) < distance)
            {
                return child;
            }

            child = child->GetNextSubnode();
        }

        return nullptr;
    };

    return (Model *)Traverse(TheWorldMgr->GetWorld()->GetRootNode());
}


bool SO::ExistNode(Node *node)
{
    if (node == nullptr)
    {
        return false;
    }

    auto Traverse = [&](Node *root) -> Node *
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        Node *child = root->GetFirstSubnode();

        while (child)
        {
            if (child == node)
            {
                return child;
            }

            child = child->GetNextSubnode();
        }

        return nullptr;
    };

    return Traverse(TheWorldMgr->GetWorld()->GetRootNode()) == node;
}
