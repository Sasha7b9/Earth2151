// 2025/01/10 19:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


enum
{
    kCollisionLandscape = (1 << 20),
    kCollisionGameObject = (1 << 30)
};


class GameWorld : public World, public Global<GameWorld>
{
public:

    GameWorld(pchar name);

    virtual ~GameWorld() override { };

    Node *FindNodeByName(pchar);

    virtual ResourceResult PreprocessWorld() override;

    virtual void BeginRendering(ToneMapper *toneMapper) override;

    void ChangeCursorPosition(float deltaX, float deltaY);

    static LocatorMarker *FindSpectatorLocator(const Zone *);

private:

    virtual void MoveWorld() override;

    void RunOnFirstFrame();

    virtual RigidBodyStatus HandleNewGeometryContact(RigidBodyController *, const GeometryContact *) override;
};


extern GameWorld *TheGameWorld;