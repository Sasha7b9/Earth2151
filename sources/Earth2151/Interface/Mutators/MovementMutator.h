// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <Resource/C4Packing.h>


enum
{
    kMutatorMovement = 'move'
};

class MovementMutator final : public Sprocket
{

    friend class SprocketReg<MovementMutator>;

public:
    MovementMutator(Point2D posShow, Point2D posHide, Vector2D speed);
    ~MovementMutator();

    void SetParameters(const Point2D &posShow, const Point2D &posHide, const Vector2D &speed);
    void Toggle();
    virtual void PreprocessSprocket() override;

    void Pack(Packer &, uint) const override;
    void Unpack(Unpacker &, uint) override;
    bool UnpackChunk(const ChunkHeader *, Unpacker &, uint);

    //        int GetSettingCount() const override;
    //        Setting *GetSetting(int index) const override;]
    virtual void CommitSetting(const Setting *) override;

    virtual void MoveSprocket() override;
    virtual void ResetSprocket() override;

private:
    MovementMutator();
    MovementMutator(const MovementMutator &movementMutator);

    MovementMutator &operator=(const MovementMutator &) = delete;

    void SetCurrenPosition();
    void CalculateNewPosition(float k);

    Sprocket *Replicate() const override;

    uint state = 0;
    Point2D positionShow = { 0.0F, 0.0F };
    Point2D positionHide = { 0.0F, 0.0F };
    Point2D positionCurrent = { 0.0F, 0.0F };
    Vector2D speed = { 0.0F, 0.0F };
};
