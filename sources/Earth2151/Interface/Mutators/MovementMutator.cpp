// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "MovementMutator.h"
#include "Utils/Math.h"


enum
{
    kShowPosition,
    kHidePosition,
    kShowToHideMoving,
    kHideToShowMoving
};

MovementMutator::MovementMutator() : Sprocket(kMutatorMovement)
{

}

MovementMutator::MovementMutator(const MovementMutator& movementMutator) : Sprocket(movementMutator)
{

}

MovementMutator::MovementMutator(Point2D posShow, Point2D posHide, Vector2D eSpeed) : Sprocket(kMutatorMovement)
{
    SetParameters(posShow, posHide, eSpeed);
}

MovementMutator::~MovementMutator()
{

}

void MovementMutator::SetParameters(const Point2D &posShow, const Point2D &posHide, const Vector2D &eSpeed)
{
    positionShow = posShow;
    positionHide = posHide;
    speed = eSpeed;
    positionCurrent = positionShow;
    state = kShowPosition;
    SetCurrenPosition();
}

void MovementMutator::Toggle()
{
    if(state == kShowPosition)
    {
        state = kShowToHideMoving;
    }
    else if(state == kHidePosition)
    {
        state = kHideToShowMoving;
    }
    else if(state == kShowToHideMoving)
    {
        state = kHideToShowMoving;
    }
    else if(state == kHideToShowMoving)
    {
        state = kShowToHideMoving;
    }
}

void MovementMutator::MoveSprocket()
{
    Sprocket::MoveSprocket();

    if(state == kShowToHideMoving)
    {
        CalculateNewPosition(1.0F);
    }
    else if(state == kHideToShowMoving)
    {
        CalculateNewPosition(-1.0F);
    }

    SetCurrenPosition();
}

void MovementMutator::CalculateNewPosition(float k)
{
    Point2D positionPrev = positionCurrent;
    positionCurrent += k * speed * TheTimeMgr->GetFloatDeltaTime();
    if(M::BetweenTwoPoints(positionPrev, positionCurrent, k > 0 ? positionHide : positionShow))
    {
        positionCurrent = k > 0 ? positionHide : positionShow;
        state = k > 0 ? kHidePosition : kShowPosition;
    }
}

void MovementMutator::SetCurrenPosition()
{
    Widget *widget = GetTargetWidget();
    if(widget)
    {
        widget->SetWidgetPosition(Point2D(round(positionCurrent.x), round(positionCurrent.y)));
        widget->InvalidateWidget();
    }
}

Sprocket *MovementMutator::Replicate() const
{
    return new MovementMutator(*this);
}

void MovementMutator::PreprocessSprocket()
{
    Sprocket::PreprocessSprocket();
    positionCurrent = positionShow;
    SetCurrenPosition();
    state = kShowPosition;
}

void MovementMutator::Pack(Packer &data, uint packFlags) const
{
    Sprocket::Pack(data, packFlags);
}

void MovementMutator::Unpack(Unpacker &data, uint unpackFlags)
{
    Sprocket::Unpack(data, unpackFlags);
}


bool MovementMutator::UnpackChunk(const ChunkHeader * /*chunkHeader*/, Unpacker& /*data*/, uint /*unpackFlags*/)
{
    return false;
}


//int MovementMutator::GetSettingCount() const
//{
//    return Mutator::GetSettingCount();
//}


//Setting *MovementMutator::GetSetting(int index) const
//{
//    int count = Mutator::GetSettingCount();
//
//    if(index < count)
//    {
//        return Mutator::GetSetting(index);
//    }
//
//    return nullptr;
//}


void MovementMutator::CommitSetting(const Setting *setting)
{
    Sprocket::CommitSetting(setting);
}


void MovementMutator::ResetSprocket()
{
    Sprocket::ResetSprocket();
}
