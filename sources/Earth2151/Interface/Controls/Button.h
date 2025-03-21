// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Controls/Canvas.h"
#include "Interface/Panels/PanelMap.h"
#include "Input/Mouse.h"


class ButtonSkin;


class TButton : public Widget
{
public:

    enum class Type
    {
        Normal,
        NameTab
    };

    TButton(Type, const Vector2D &position, pchar);
    ~TButton();

    virtual void MoveWidget() override { };
    virtual void UpdateWidget() override { };
    void SetSelected(bool selected);        // Only for TypeNameTab

    void HandleObserver(MouseObservable *, uint);

protected:
    Type buttonType = Type::Normal;

private:
    Array<ButtonSkin *> skins;
    bool selected = false;
    Observer<TButton, MouseObservable> observer;
    WidgetEventData eventData;

    void SetState(uint buttonState);
};


class ButtonSkin : public Canvas
{
public:

    enum class State
    {
        Normal,
        UnderMouse,
        Pressed
    };

    ButtonSkin(TButton::Type type, pchar text, State state);

private:
    void DrawSkin(TButton::Type type, const ColorRGBA &colorBackground, const ColorRGBA &colorBorder);
};
