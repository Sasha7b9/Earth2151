// 2025/02/27 14:07:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/World/Sun.h"
#include "Graphics/Primitives.h"


namespace Sun
{
    static InfiniteLight *light = nullptr;
    static ColorRGBA color;
    static Transform3D transform;
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
                light = (InfiniteLight *)l;

                InfiniteLightObject *object = light->GetObject();

                float b = 10.0f;

                color.Set(b, b, b * 3.0f / 4.0f);

                object->SetLightColor(color);

                transform = light->GetNodeTransform();

                return;
            }
        }

        subnode = node->GetNextTreeNode(subnode);

    } while (subnode);

    light = new InfiniteLight(color);
    light->SetNodeTransform(transform);
    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(light);
}


void Sun::Update()
{

}
