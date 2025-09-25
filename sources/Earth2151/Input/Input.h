// 2025/02/14 23:06:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Cameras.h"


namespace TypeAction
{
    // NavigationAction
    const uint Forward = 'frwd';        // W
    const uint Backward = 'bkwd';       // S
    const uint Left = 'left';           // A
    const uint Right = 'rght';          // D

    const uint RotLeft = 'rotl';        // Num 4
    const uint RotRight = 'rotr';       // Num 6
    const uint RotTop = 'rott';         // Num 9
    const uint RotDown = 'rotd';        // Num 3
    const uint ZoomIn = 'zmin';         // Num 2
    const uint ZoomOut = 'zmot';        // Num 8
    const uint Center = 'cntr';         // Num 5    направить камеру на центр сцены

    // KeyboardAction
    const uint Key1 = 'key1';
    const uint Key2 = 'key2';
    const uint Key3 = 'key3';
    const uint Key4 = 'key4';
    const uint F1 = 'kF1';              // "F1"
    const uint F2 = 'kF2';              // "F2"
    const uint Ctrl = 'ctrl';           // "Ctrl"

    // MouseAction
    const uint MouseLeft = 'lbtn';      // "Кнопка 0"
    const uint MouseRight = 'mbtn';     // "Кнопка 1"
    const uint MouseMiddle = 'rbtn';    // "Кнопка 2"
    const uint MouseWheel = 'weel';     // "Колесико"

    // System
    const uint GamePause = 'paus';     // "Num Lock"
}


class NavigationAction : public Action
{
public:

    NavigationAction(ActionType type, CameraRTS::E flag) : Action(type) { movementFlag = flag; }
    virtual ~NavigationAction() { }
    virtual void HandleEngage() override
    {
        TheCameraRTS->AddNavFlag(movementFlag);
    }
    virtual void HandleDisengage() override { TheCameraRTS->RemoveNavFlag(movementFlag); }

private:
    CameraRTS::E movementFlag;
};


class KeyboardAction : public Action
{
public:
    KeyboardAction(ActionType type) : Action(type) { }
    virtual ~KeyboardAction() {    }
    virtual void HandleEngage() override;
    virtual void HandleDisengage() override;
};


class MouseButtonAction : public Action
{
public:
    MouseButtonAction(ulong type) : Action(type) { };
    virtual ~MouseButtonAction() { }
    virtual void HandleEngage() override;
    virtual void HandleDisengage() override;
    void Update(float value);

};


namespace Input
{
    void Init();

    extern bool keyCtlr;            // true, если нажата Ctrl
}
