// 2025/02/14 22:34:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Widgets/GameCursor.h"
#include "Interface/Controls/Canvas.h"
#include "Cameras.h"
#include "Utils/Math.h"


const float GameCursor::SPEED_ANIMATION = 5.0f * 1e-2f;


GameCursor *TheGameCursor = nullptr;


GameCursor::GameCursor() : Widget(), Global<GameCursor>(TheGameCursor)
{
    position = TheEngine->GetWindowResolution() / 2;

    for(int st = 0; st < State::Count; st++)
    {
        for(int fr = 0; fr < NUM_FRAMES; fr++)
        {
            skins[st][fr] = nullptr;
        }
    }

    mainWidget = new Widget();

    PrepareSkins();

    AppendNewSubnode(mainWidget);

    state = State::Normal;

    SetSkin(0);
}

GameCursor::~GameCursor()
{
    for(int st = 0; st < State::Count; st++)
    {
        for(int fr = 0; fr < NUM_FRAMES; fr++)
        {
            delete skins[st][fr];
        }
    }
}


void GameCursor::MoveWidget()
{
    Vector2D speed = TheCameraRTS->GetSpeed();

    if (speed == Vector2D(0, 0))
    {
        state = State::Normal;
//        state = GO::GetFromScreen(position)->GetGameObjectProperty().Selectable() ? State::OverObject : State::Normal;
    }
    else
    {
        if (fabs(speed.y) < Math::epsilon)
        {
            state = speed.x < 0.0f ? State::OutScreenLeft : State::OutScreenRight;
        }
        else if (fabs(speed.x) < Math::epsilon)
        {
            state = speed.y > 0.0f ? State::OutScreenDown : State::OutScreenUp;
        }
        else if (speed.y < 0.0f)
        {
            state = speed.x < 0.0f ? State::OutScreenLeftUp : State::OutScreenRightUp;
        }
        else if (speed.y > 0.0f)
        {
            state = speed.x < 0.0f ? State::OutScreenLeftDown : State::OutScreenRightDown;
        }
    }

    float dT = TheTimeMgr->GetFloatDeltaTime();
    currentFrame += dT * SPEED_ANIMATION;

    while(currentFrame > (NUM_FRAMES - 1))
    {
        currentFrame -= NUM_FRAMES;
    }

    int frame = (int)currentFrame;
 
   SetSkin(frame);
}


void GameCursor::PrepareSkins()
{
    for(int i = 0; i < NUM_FRAMES; i++)
    {
        skins[State::Normal][i] = CreateNormalOverSkin(State::Normal, i);
        skins[State::OverObject][i] = CreateNormalOverSkin(State::OverObject, i);
    }

    for(int s = State::OutScreenLeft; s <= State::OutScreenLeftDown; s++)
    {
        for(int i = 0; i < NUM_FRAMES; i++)
        {
            skins[s][i] = CreateOutScreenSkin((State::E)s, i);
        }
    }

    for (int s = State::ZoomIn; s < State::Count; s++)
    {
        for (int i = 0; i < NUM_FRAMES; i++)
        {
            skins[s][i] = CreateMouseDownSkin((State::E)s, i);
        }
    }
}


Canvas *GameCursor::CreateNormalOverSkin(State::E eState, int numFrame)
{
    float size = 40;

    float component = 0.0f;

    Point2D points[3] = {
        {0.0F, 0.0F},
        {(float)(size - 1), (float)(size / 2)},
        {(float)(size / 2), (float)(size - 1)}
    };

    Canvas *skin = new Canvas(Vector2D(size, size));
    ColorRGBA color(component, component, component);
    skin->ClearAndFillRegion(points, 3, color, M::OneComplement(color), Point2D(5, 5));
    skin->EndPaint();

    float radiusMax = size / (eState == State::Normal ? 3.0f : 2.0f);
    float radiusMin = size / 10.0f;
    float centerX = size / 2.0f;
    float centerY = size / 2.0f;
    int shiftCircle = (int)(size / 3.5f);
    Canvas *circle = new Canvas(Vector2D(size, size));
    circle->SetColorBackground(ColorRGBA(0.0, 0.0, 0.0, 0.0f));
    circle->Clear();
    circle->SetColorBrush(eState == State::Normal ? Color::black : ColorRGBA(0.25f, 0.25f, 0.25f));
    circle->FillCircle(centerX, centerY, radiusMax);
    circle->SetColorBrush(Color::white);
    circle->DrawCircle(centerX, centerY, radiusMax);
    circle->DrawCircle(centerX, centerY, radiusMin);

    int numLines = 6;
    float stepAngle = Math::two_pi / (float)(numLines);
    float frameAngle = Math::two_pi / NUM_FRAMES * (float)(numFrame) / (float)(numLines);

    for(int i = 0; i < numLines; i++)
    {
        float angle = frameAngle + (float)(i) * stepAngle;
        float lenMin = radiusMin * 1.5f;
        float lenMax = radiusMax * 0.8f;
        float xStart = centerX + Cos(angle) * lenMin + 0.5f;
        float yStart = centerY + Sin(angle) * lenMin + 0.5f;
        float xEnd = centerX + Cos(angle) * lenMax + 0.5f;
        float yEnd = centerY + Sin(angle) * lenMax + 0.5f;
        circle->DrawLine(xStart, yStart, xEnd, yEnd);
    }

    circle->EndPaint();

    skin->AppendNewSubnode(circle);

    Transform3D transform = circle->GetWidgetTransform();
    transform.SetTranslation((float)shiftCircle, (float)shiftCircle, 0.0f);
    circle->SetWidgetTransform(transform);

    return skin;
}


Canvas *GameCursor::CreateMouseDownSkin(State::E, int)
{
    return nullptr;
}


Canvas *GameCursor::CreateOutScreenSkin(State::E _state, int /*numFrame*/)
{
    float width = 100.0f;
    float height = 51.0f;

    float component = 1.0f;
    float delta = 5.0f;

    Canvas *skin = new Canvas(Vector2D(width, height));
    skin->SetColorBackground( {0.0f, 0.0f, 0.0f, 0.0f} );
    skin->Clear();
    ColorRGBA color(component, component, component);
    skin->SetColorBrush(color);

    Point2D topLef{5, 5};
    Point2D topRight{50, 5};
    Point2D downRight{50, 50};
    Point2D downLeft{5, 50};

    Point2D left{5, 25};
    Point2D right{50, 25};
    Point2D top{25, 5};
    Point2D down{25, 50};

    if(_state == State::OutScreenLeft)
    {
        skin->DrawLine(left, right);
        skin->DrawLine(left, top);
        skin->DrawLine(left, down);
    }
    else if(_state == State::OutScreenLeftUp)
    {
        skin->DrawLine(topLef, left);
        skin->DrawLine(topLef, downRight);
        skin->DrawLine(topLef, top);
    }
    else if(_state == State::OutScreenUp)
    {
        skin->DrawLine(top, down);
        skin->DrawLine(top, left);
        skin->DrawLine(top, right);
    }
    else if(_state == State::OutScreenDown)
    {
        skin->DrawLine(top, down);
        skin->DrawLine(left, down);
        skin->DrawLine(right, down);
        skin->SetWidgetPosition({ 0.0f, -height - delta });
    }
    else if(_state == State::OutScreenRightUp)
    {
        skin->DrawLine(downLeft, topRight);
        skin->DrawLine(top, topRight);
        skin->DrawLine(topRight, right);
        skin->SetWidgetPosition({ -height - delta, 0.0f });
    }
    else if(_state == State::OutScreenRight)
    {
        skin->DrawLine(left, right);
        skin->DrawLine(top, right);
        skin->DrawLine(down, right);
        skin->SetWidgetPosition({ -55.0f, 0.0f });
    }
    else if (_state == State::OutScreenRightDown)
    {
        skin->DrawLine(topLef, downRight);
        skin->DrawLine(topRight, downRight);
        skin->DrawLine(downLeft, downRight);
        skin->SetWidgetPosition({ -55.0f, -height - delta });
    }
    else if (_state == State::OutScreenLeftDown)
    {
        skin->DrawLine(topLef, downLeft);
        skin->DrawLine(topRight, downLeft);
        skin->DrawLine(downRight, downLeft);
        skin->SetWidgetPosition({ 0.0f, -height - delta });
    }
    else
    { 
        skin->DrawCircle(20, 20, 10);
    }

    skin->EndPaint();

    return skin;
}

void GameCursor::UpdateWidget()
{

}

void GameCursor::SetSkin(int numSkin)
{
    mainWidget->RemoveSubtree();
    mainWidget->AppendSubnode(skins[state][numSkin]);
}

CursorGUIMutator::CursorGUIMutator() : Sprocket(kMutatorCursorGUI)
{

}

CursorGUIMutator::~CursorGUIMutator()
{
//    float dT = TheTimeMgr->GetFloatDeltaTime();
//    static_cast<GameCursor*>(GetTargetWidget())->currentFrame += dT * 
}

void CursorGUIMutator::MoveSprocket()
{

}

void CursorGUIMutator::ResetSprocket()
{

}
