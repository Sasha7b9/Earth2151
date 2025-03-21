// 2025/01/11 11:33:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Graphics/Primitives.h"
#include "Graphics/Materials.h"


Node *Sphere::CreateNew(const Point3D &coord, float radius, const ColorRGBA &color, pchar node_name)
{
    PrimitiveGeometry *geometry = new SphereGeometry({ radius, radius, radius });
    PrimitiveGeometryObject *object = geometry->GetObject();
    object->collisionExclusionMask = kCollisionExcludeAll;
    object->geometryFlags &= ~kGeometryCastShadows;
    object->SetBuildLevelCount(1);
    geometry->UpdateNode();
    object->BuildPrimitive(geometry);
    geometry->UpdateNode();

    MaterialObject *material = new MaterialObject();
    DiffuseAttribute *diffuse = new DiffuseAttribute(color);
    material->AddAttribute(diffuse);
    geometry->SetMaterialObject(0, material);
    material->Release();

    geometry->SetNodePosition(coord);

    geometry->SetNodeName(node_name);

    return geometry;
}


Node *Disk::CreateNew(float radius, const ColorRGBA &color)
{
    PrimitiveGeometry *geometry = new DiskGeometry({ radius, radius });
    PrimitiveGeometryObject *object = geometry->GetObject();
    object->collisionExclusionMask = kCollisionExcludeAll;
    object->geometryFlags &= ~kGeometryCastShadows;
    object->SetBuildLevelCount(1);
    object->BuildPrimitive(geometry);

    MaterialObject *material = new MaterialObject();
    DiffuseAttribute *diffuse = new DiffuseAttribute(color);
    material->materialFlags |= kMaterialTwoSided | kMaterialAlphaTest | kMaterialAlphaCoverage;
    material->AddAttribute(diffuse);
    geometry->SetMaterialObject(0, material);
    material->Release();

    return geometry;
}


WorldGizmo::WorldGizmo(float _size, float _radius) : Node('wlgz'), size(_size), radius(_radius)
{
    matRed = Materials::Create(TypeMaterial::Diffuse, Color::red);

    AppendNewSubnode(CreateAxis(matRed, Transform3D::MakeRotationY(Math::pi / 2), { 0.0f,        -radius / 2, radius / 2 }));

    matGreen = Materials::Create(TypeMaterial::Diffuse, Color::green);

    AppendNewSubnode(CreateAxis(matGreen, Transform3D::MakeRotationX(-Math::pi / 2), { -radius / 2, 0.0f,        radius / 2 }));

    matBlue = Materials::Create(TypeMaterial::Diffuse, Color::blue);

    AppendNewSubnode(CreateAxis(matBlue, Transform3D::MakeRotationZ(Math::pi / 2), { radius / 2,  -radius / 2, 0.0f }));
}


WorldGizmo::~WorldGizmo()
{
    PurgeSubtree();
    RemoveSubtree();

    matRed->PurgeAttributes();
    matGreen->PurgeAttributes();
    matBlue->PurgeAttributes();

    delete matRed;
    delete matGreen;
    delete matBlue;
}


Node *WorldGizmo::CreateAxis(MaterialObject *mat, const Transform3D &transform, const Point3D &position)
{
    PrimitiveGeometry *geometry = new PyramidGeometry({ radius, radius }, size);
    PrimitiveGeometryObject *object = geometry->GetObject();
    object->geometryFlags &= ~kGeometryCastShadows;
    object->BuildPrimitive(geometry);

    geometry->SetMaterialObject(0, mat);

    geometry->SetNodeTransform(transform);

    geometry->SetNodePosition(position);

    return geometry;
}


void WorldGizmo::PreprocessNode()
{
    Node::PreprocessNode();
    UpdateNode();
}
