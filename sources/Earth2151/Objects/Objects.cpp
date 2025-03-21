// 2025/02/02 17:38:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Objects/Objects.h"
#include "Graphics/Primitives.h"
#include "Earth2150/Parameters/Models/Passive.h"
#include "Earth2150/Files/Model2150.h"


GenericObject::~GenericObject()
{
    if (task)
    {
        delete task;
        task = nullptr;
    }
}


void GenericObject::OnEventRebuild()
{
    ControllerAS::OnEventRebuild();

    READ_FLOAT(scale);

    SetNodeScale(scale);
}


void GenericObject::PreprocessController()
{
    ControllerAS::PreprocessController();

    SetNodeScale(scale);

    AppendGizmo();
}


void GenericObject::AppendGizmo()
{
    WorldGizmo *gizmo = new WorldGizmo(1.0f, 0.03f);

    gizmo->SetNodeRotation(Transform3D::MakeRotationZ(GTR(-90.0f)));

    GetTargetNode()->AppendNewSubnode(gizmo);

    if (!g_showing_gizmo)
    {
        gizmo->DisableNode();
    }
}


void GenericObject::SetNodeScale(float scale)
{
    Model *model = (Model *)GetTargetNode();

    if (model)
    {
        model->UpdateNode();

        Node *node = model;
        Node *subnode = node;

        int counter_light = 0;

        do
        {
            NodeType type = subnode->GetNodeType();

            if (type == kNodeGeometry && scale != 1.0f)
            {
                Geometry *geometry = (Geometry *)subnode;

                {
                    for (int i = 0; i < geometry->GetMaterialCount(); i++)
                    {
                        MaterialObject *mat = geometry->GetMaterialObject(i);

                        mat->materialFlags |= kMaterialTwoSided;
                    }

                    geometry->InvalidatePipelineData();
                    geometry->InvalidateAmbientPipelineData();
                }

                GeometryObject *object = geometry->GetObject();

                int count = object->GetGeometryLevelCount();

                for (int i = 0; i < count; i++)
                {
                    Mesh *mesh = object->GetGeometryLevel(i);

                    mesh->ScaleMesh({ scale, scale, scale });
                }

                ((GenericGeometryObject *)object)->RebuildGeometry(geometry);

                object->BuildCollisionData();

                geometry->InvalidateNode();
                geometry->NeutralizeNode();
                geometry->PreprocessNode();
            }

            if (type == kNodeLight)
            {
                if (counter_light > 3)
                {
                    subnode->DisableNode();
                }

                counter_light++;
            }

            subnode->InvalidateNode();

            subnode = node->GetNextTreeNode(subnode);

        } while (subnode);

        model->InvalidateNode();

        model->UpdateNode();
    }
}


float GenericObject::CalculateAngle(const Vector3D &vector1, const Vector3D &vector2)
{
    Vector3D vec1 = vector1;
    vec1.Normalize();
    Vector3D vec2 = vector2;
    vec2.Normalize();

    float angle = Arccos(vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);

    return angle;
}


void MovedObject::OnEventRebuild()
{
    GenericObject::OnEventRebuild();

    READ_FLOAT(acceleration_speed);
    READ_FLOAT(max_speed);
    READ_FLOAT(slow_speed);
    READ_FLOAT(rotate_speed);
}


void MovedObject::Rotate(const Vector3D &axis, const Vector3D &dir_on_target)
{
    // Смещаем направление к цели
    {
        Vector3D a = axis;
        a.Normalize();
        Bivector3D vec;
        vec.Set(a.x, a.y, a.z);

        float angle = -GTR(GetRotateSpeed()) * FDELTA_SEC;

        float angle_target = CalculateAngle(dir_on_target, GetDirectionForward());

        if (std::fabs(angle) > angle_target)
        {
            angle = (angle > 0.0f) ? angle_target : -angle_target;
        }

        direction = Matrix3D::MakeRotation(angle, vec) * Matrix3D().SetIdentity() * direction;

        direction.Normalize();
    }

    up = axis;
    up.Normalize();

    Vector3D _direction = GetCompensatedDirection(direction);

    Vector3D rigth = Cross(_direction, up).Normalize();

    GetTargetNode()->SetNodeMatrix3D({ rigth, _direction, up });
    GetTargetNode()->InvalidateNode();
}


void MovedObject::_RotateAboutAxis(const Vector3D &axis, float dir)
{
    Bivector3D up_vec;
    up_vec.Set(axis.x, axis.y, axis.z);

    direction.RotateAboutAxis(dir * GTR(GetRotateSpeed()) * FDELTA_SEC, up_vec);     // Повернули направление
    direction.Normalize();

    up.RotateAboutAxis(dir * GTR(GetRotateSpeed()) * FDELTA_SEC, up_vec);            // Повернули "взгляд вверх"
    up.Normalize();

    Vector3D _direction = GetCompensatedDirection(direction);

    Vector3D right = Cross(_direction, up).Normalize();
    GetTargetNode()->SetNodeMatrix3D({ right, _direction, up });
    GetTargetNode()->InvalidateNode();
}


void MovedObject::SetNodeDirection(const Vector3D &new_dir, const Vector3D &new_up)
{
    direction = new_dir;
    direction.Normalize();
    up = new_up;
    up.Normalize();

    Vector3D dir = GetCompensatedDirection(direction);

    Vector3D right = Cross(dir, up).Normalize();
    GetTargetNode()->SetNodeMatrix3D({ right, dir, up });
    GetTargetNode()->InvalidateNode();
}


Vector3D MovedObject::GetCompensatedDirection(const Vector3D &_dir)
{
    Bivector3D bi_up;
    bi_up.Set(up.x, up.y, up.z);

    Vector3D vector = _dir;
    return vector.RotateAboutAxis(GTR(90.0f), bi_up);
}


void MovedObject::PreprocessController()
{
    GenericObject::PreprocessController();

    direction = GetTargetNode()->GetWorldTransform()[0];
    direction.Normalize();
}


void MovedObject::Move()
{
    GenericObject::Move();
}


void MovedObject::MoveForward(float direction)
{
    Vector3D offset = GetDirectionForward() * speed * FDELTA_SEC;

    GetTargetNode()->SetNodePosition(GetTargetNode()->GetNodePosition() + offset);

    GetTargetNode()->InvalidateNode();

    speed += ((direction >= 0.0f) ? GetAccelerationSpeed() : -GetSlowSpeed()) * FDELTA_SEC;

    if (speed > GetMaxSpeed())
    {
        speed = GetMaxSpeed();
    }
    else if (speed < 0.0f)
    {
        speed = 0.0f;
    }
}
