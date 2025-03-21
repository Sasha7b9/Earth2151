// 2025/01/11 11:33:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Sphere
{
    static Node *CreateNew(const Point3D &, float radius, const ColorRGBA &color, pchar node_name);
};


struct Disk
{
    static Node *CreateNew(float radius, const ColorRGBA &);
};


class WorldGizmo : public Node
{
public:

    WorldGizmo(float size = 1500.0f, float radius = 0.05f);
    ~WorldGizmo();

    virtual void PreprocessNode() override;

private:

    Node *CreateAxis(MaterialObject *, const Transform3D &, const Point3D &position);
    float size = 0.0f;
    float radius = 0.05f;
    MaterialObject *matRed = nullptr;
    MaterialObject *matGreen = nullptr;
    MaterialObject *matBlue = nullptr;
};
