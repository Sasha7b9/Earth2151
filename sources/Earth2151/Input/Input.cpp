// 2025/02/14 23:06:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Input.h"
#include "Mouse.h"
#include "GameWorld.h"
#include "Game/Game.h"


bool Input::keyCtlr = false;


void Input::Init()
{
    TheInputMgr->AddAction(new NavigationAction(TypeAction::Left, CameraRTS::E::MoveLeft));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::Right, CameraRTS::E::MoveRight));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::Forward, CameraRTS::E::MoveForward));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::Backward, CameraRTS::E::MoveBackward));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::RotLeft, CameraRTS::E::RotateLeft));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::RotRight, CameraRTS::E::RotateRight));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::RotTop, CameraRTS::E::RotateTop));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::RotDown, CameraRTS::E::RotateDown));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::ZoomIn, CameraRTS::E::ZoomIn));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::ZoomOut, CameraRTS::E::ZoomOut));
    TheInputMgr->AddAction(new NavigationAction(TypeAction::Center, CameraRTS::E::MoveCenter));

    TheInputMgr->AddAction(new KeyboardAction(TypeAction::F1));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::F2));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::Ctrl));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::Key1));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::Key2));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::Key3));
    TheInputMgr->AddAction(new KeyboardAction(TypeAction::Key4));

    TheInputMgr->AddAction(new MouseButtonAction(TypeAction::MouseLeft));
    TheInputMgr->AddAction(new MouseButtonAction(TypeAction::MouseMiddle));
    TheInputMgr->AddAction(new MouseButtonAction(TypeAction::MouseRight));
    TheInputMgr->AddAction(new MouseButtonAction(TypeAction::MouseWheel));
}


void KeyboardAction::HandleEngage()
{
    switch (GetActionType())
    {
    case TypeAction::Ctrl:
        Input::keyCtlr = true;
        break;

    case TypeAction::Key1:
        GameWorld::Set(Race::UCS);
        break;

    case TypeAction::Key2:
        GameWorld::Set(Race::ED);
        break;

    case TypeAction::Key3:
        GameWorld::Set(Race::LC);
        break;

    case TypeAction::Key4:
        GameWorld::Set(0);                  // В этом мире происходит игра
        break;
    }
}


void KeyboardAction::HandleDisengage()
{
    switch (GetActionType())
    {
    case TypeAction::Ctrl:
        Input::keyCtlr = false;
        break;
    }
}


void MouseButtonAction::HandleEngage()
{
    ActionType type = GetActionType();

    if (type == TypeAction::MouseLeft)
    {
        TheMouse->SetLeftPressed();
    }
    else if (type == TypeAction::MouseMiddle)
    {
        TheMouse->SetMiddlePressed();
    }
    else if (type == TypeAction::MouseRight)
    {
        TheMouse->SetRightPressed();
    }
}

void MouseButtonAction::HandleDisengage()
{
    ActionType type = GetActionType();

    if (type == TypeAction::MouseLeft)
    {
        TheMouse->SetLeftReleased();
    }
    else if (type == TypeAction::MouseMiddle)
    {
        TheMouse->SetMiddleReleased();
    }
    else if (type == TypeAction::MouseRight)
    {
        TheMouse->SetRightReleased();
    }
}

void MouseButtonAction::Update(float value)
{
    uint type = GetActionType();

    if (type == TypeAction::MouseWheel)
    {
        if (value > 0)
        {
            TheCameraRTS->TiltAngleDecrease();
        }
        else
        {
            TheCameraRTS->TiltAngleIncrease();
        }
    }
}
