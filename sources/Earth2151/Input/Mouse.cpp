// 2025/02/14 23:06:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Input/Mouse.h"
#include "Interface/Widgets/GameCursor.h"
#include "Interface/GUI.h"
#include "Cameras.h"


Mouse *TheMouse = nullptr;


Mouse::Mouse() : Global<Mouse>(TheMouse)
{
}


MouseObservable::~MouseObservable()
{
}


void Mouse::AddObserver(Observer<TButton, MouseObservable> *observer)
{
    observable.AddObserver(observer);
}


void Mouse::AddObserver(Observer<PanelMap, MouseObservable> *observer)
{
    observable.AddObserver(observer);
}


void Mouse::SetLeftPressed()
{
    leftNowPressed = true;
    leftIsPressed = true;
    leftNowReleased = false;

    TheGUI->MouseLeftIsPress();
}


void Mouse::SetRightPressed()
{
    rightIsPressed = true;
    observable.PostEvent((int)Event::RightChanged);

    transform_camera_begin_right_down = TheCameraRTS->GetWorldTransform();
}


void Mouse::SetRightReleased()
{
    rightIsPressed = false;
    observable.PostEvent((int)Event::RightChanged);
}


void Mouse::SetLeftReleased()
{
    bool prevLeftReleased = leftNowReleased;
    leftNowReleased = true;
    leftNowPressed = false;
    leftIsPressed = false;

    if (!prevLeftReleased)
    {
        observable.PostEvent((int)Event::LeftChanged);
    }

    TheGUI->MouseLeftIsRelease();
}
