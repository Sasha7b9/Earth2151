// 2025/02/27 14:07:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Sun.h"
#include "Graphics/Primitives.h"


namespace Sun
{
    static Light *light = nullptr;
}


void Sun::Init()
{
    Node *node = TheWorldMgr->GetWorld()->GetRootNode();
    Node *subnode = node;

    do 
    {
        if (subnode->GetNodeType() == kNodeLight)
        {
            Light *l = (Light *)subnode;

            if (l->GetLightType() == kLightInfinite)
            {
                light = l;

                LightObject *object = l->GetObject();

                float b = 10.0f;

                object->SetLightColor({ b, b, b * 3.0f / 4.0f });

                break;
            }
        }

        subnode = node->GetNextTreeNode(subnode);

    } while (subnode);
}


void Sun::Update()
{

}
