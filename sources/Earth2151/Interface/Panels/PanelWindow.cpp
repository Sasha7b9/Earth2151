// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Panels/PanelWindow.h"
#include "Interface/Controls/Button.h"
#include "Interface/Mutators/MovementMutator.h"
#include "Settings.h"
#include "Sounds/SoundPlayer.h"
#include "Utils/Math.h"


PanelWindow::PanelWindow(const Vector2D &size) : Canvas(size)
{
    Clear();

    EndPaint();
}

PanelWindow::~PanelWindow()
{

}

void PanelWindow::PreprocessWidget()
{
    Canvas::PreprocessWidget();

//    RootWidget *root = GetRootWidget();
//
//    GetRootWidget()->AddMovingWidget(this);
}

void PanelWindow::SetMovementMutator(const Point2D &posShow, const Point2D &posHide, const Vector2D &speed)
{
    Sprocket *mutator = GetFirstSprocket();
    while (mutator)
    {
        if (mutator->GetSprocketType() == kMutatorMovement)
        {
            static_cast<MovementMutator *>(mutator)->SetParameters(posShow, posHide, speed);
            break;
        }
        mutator = mutator->GetNextListElement();
    }
    if (!mutator)
    {
        MovementMutator *_mutator = new MovementMutator(posShow, posHide, speed);
        _mutator->SetSprocketState(0);
        AddSprocket(_mutator);
    }
}

void PanelWindow::Toggle()
{
    TheSoundPlayer->Play(kMovePanel);
    Sprocket *mutator = GetFirstSprocket();

    while (mutator)
    {
        if (mutator->GetSprocketType() == kMutatorMovement)
        {
            static_cast<MovementMutator *>(mutator)->Toggle();
            break;
        }
        mutator = mutator->GetNextListElement();
    }
}

void PanelWindow::Clear()
{
    SetColorBackground(Color::black);
    Canvas::Clear();
    SetColorBrush(Color::cyan);
    DrawRectangle({0.0f, 0.0f}, {GetWidgetSize().x - 1, GetWidgetSize().y - 1});
}


bool PanelWindow::UnderMouse() const
{
    Point3D position = GetWorldPosition();

    Vector2D size = GetWidgetSize();

    Point2D positionMouse = TheMouse->GetPosition();

    return M::PointInRect(&positionMouse, position.x, position.y, size.x, size.y);
}
