// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Controls/Button.h"
#include "Settings.h"
#include "Utils/Math.h"
#include "Interface/Widgets/GameCursor.h"


ButtonSkin::ButtonSkin(TButton::Type type, pchar text, State state) 
   : Canvas(SET::GUI::BUTTON::SIZE(type))
{
    ColorRGBA _colorBackground(SET::GUI::BUTTON::COLOR::BACKGROUND(type));
    ColorRGBA _colorBrush(SET::GUI::COLOR::BORDER());
    ColorRGBA colorText(1.0f, 1.0f, 1.0f);
    if (state == ButtonSkin::State::UnderMouse)
    {
        _colorBackground = M::SetColorBrightness(_colorBackground, type == TButton::Type::Normal ? 0.2f : 0.3f);
    }
    else if (state == ButtonSkin::State::Pressed) {
        _colorBackground = M::SetColorBrightness(_colorBackground, type == TButton::Type::Normal ? 0.4f : 0.6f);
        colorText = ColorRGBA(0.0f, 0.0f, 0.0f);
    }

    DrawSkin(type, _colorBackground, _colorBrush);

    TextWidget *textWidget = new TextWidget(SET::GUI::BUTTON::SIZE(type), text, "fonts/Techno28_14");

    textWidget->SetTextAlignment(Slug::kAlignmentCenter);
    textWidget->SetWidgetColor(colorText);
    textWidget->SetWidgetPosition({ 0, 2 });

    if(type == TButton::Type::NameTab)
    {
        textWidget->SetWidgetPosition({ 0, 4 });
    }

    AppendNewSubnode(textWidget);

    EndPaint();
}

void ButtonSkin::DrawSkin(TButton::Type type, const ColorRGBA &_colorBackground, const ColorRGBA &colorBorder)
{
    SetColorBackground(_colorBackground);
    Clear();
    SetColorBrush(colorBorder);
    DrawRectangle({ 0.0f, 0.0f }, { SET::GUI::BUTTON::WIDTH(type) - 1.0f, SET::GUI::BUTTON::HEIGHT(type) - 1.0f });
}

TButton::TButton(Type type, const Vector2D &position, pchar text) : Widget()
{
    buttonType = type;
    skins.AppendArrayElement(new ButtonSkin(type, text, ButtonSkin::State::Normal));
    skins.AppendArrayElement(new ButtonSkin(type, text, ButtonSkin::State::UnderMouse));
    skins.AppendArrayElement(new ButtonSkin(type, text, ButtonSkin::State::Pressed));
    AppendNewSubnode(skins[(int)ButtonSkin::State::Normal]);
    AppendNewSubnode(skins[(int)ButtonSkin::State::UnderMouse]);
    AppendNewSubnode(skins[(int)ButtonSkin::State::Pressed]);
    SetWidgetPosition(Point2D(position));
    SetState((uint)ButtonSkin::State::Normal);

    TheMouse->AddObserver(&observer);
}

TButton::~TButton()
{
    SAFE_DELETE(skins[(int)ButtonSkin::State::Normal]);
    SAFE_DELETE(skins[(int)ButtonSkin::State::UnderMouse]);
    SAFE_DELETE(skins[(int)ButtonSkin::State::Pressed]);
}

void TButton::HandleObserver(MouseObservable *, uint)
{
    Point3D pos = GetWorldPosition();
    bool mouseOnButton = M::PointInRect(&(TheGameCursor->position), pos.x, pos.y, (float)SET::GUI::BUTTON::WIDTH(buttonType), (float)SET::GUI::BUTTON::HEIGHT(buttonType));

    if (!mouseOnButton) 
    {
        if ((buttonType == TButton::Type::NameTab) && selected) 
        {
        } 
        else 
        {
            SetState((uint)ButtonSkin::State::Normal);
        }
        return;
    }
    if (buttonType == TButton::Type::Normal) 
    {
        SetState(TheMouse->LeftIsPressed() ? (uint)ButtonSkin::State::Pressed : (uint)ButtonSkin::State::UnderMouse);
    } 
    else if (buttonType == TButton::Type::NameTab) 
    {
        SetState(selected ? (uint)ButtonSkin::State::Pressed : (uint)ButtonSkin::State::UnderMouse);
    }

    if (TheMouse->LeftNowReleased())
    {
        if (buttonType == TButton::Type::NameTab) 
        {
            SetSelected(true);
        }
        SetState(buttonType == TButton::Type::NameTab ? (uint)ButtonSkin::State::Pressed : (uint)ButtonSkin::State::UnderMouse);
        this->selected = false;
        PostEvent(&eventData);
        TheMouse->SetLeftReleased();
    }
}

void TButton::SetState(uint state) 
{
    Widget *widget = GetFirstSubnode();

    while (widget) 
    {
        Widget *node = widget;
        widget = widget->GetNextSubnode();
        Widget::RemoveSubnode(node);
    }

    AppendSubnode(skins[state]);
    TButton::UpdateWidget();
    InvalidateWidget();
}

void TButton::SetSelected(bool eSelected) 
{
    selected = eSelected;
    if (selected) 
    {
        SetState((uint)ButtonSkin::State::Pressed);
    } else 
    {
        SetState((uint)ButtonSkin::State::Normal);
    }
}
