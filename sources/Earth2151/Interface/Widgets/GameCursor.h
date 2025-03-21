// 2025/02/14 22:34:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Canvas;


enum
{
    kMutatorCursorGUI = 'curg'
};


class GameCursor : public Widget, public Global<GameCursor>
{
public:

    struct State
    {
        enum E
        {
            Normal,
            OverObject,                 // Курсор находится над объектом, котоырй может быть выделен
            OutScreenLeft,              // |
            OutScreenRight,             // |
            OutScreenUp,                // |
            OutScreenDown,              // | Эти курсоры для случая, когда камера
            OutScreenLeftUp,            // | движется в сторону под действием курсора
            OutScreenRightUp,           // |
            OutScreenRightDown,         // |
            OutScreenLeftDown,          // /
            ZoomIn,                     // Курсор приближения камеры
            ZoomOut,                    // Курсор удаления камеры
            RotateLeft,                 // Поворот камеры в одну сторону
            RotateRight,                // Поворот камеры в другую сторону
            Count
        };
    };

    GameCursor();
    ~GameCursor();

    Point2D position;                           // Позиция игрового курсора на экране

    virtual void MoveWidget() override;

protected:
    virtual void UpdateWidget() override;

private:
    State::E state = State::E::Normal;
    Widget *mainWidget = nullptr;

    float currentFrame = 0.0f;
    static const int NUM_FRAMES = 50;
    static const float SPEED_ANIMATION;      // frames per second
    Canvas *skins[State::Count][NUM_FRAMES];

    void SetSkin(const int numSkin);
    void PrepareSkins();
    Canvas *CreateNormalOverSkin(const State::E state, const int numFrame);
    Canvas *CreateOutScreenSkin(const State::E state, const int numFrame);
    Canvas *CreateMouseDownSkin(const State::E state, const int numFrame);
};

class CursorGUIMutator final : public Sprocket
{
    friend class SprocketReg<CursorGUIMutator>;

public:

    CursorGUIMutator();
    ~CursorGUIMutator();

    void MoveSprocket() override;
    void ResetSprocket() override;

private:

    CursorGUIMutator(const CursorGUIMutator &);
};


extern GameCursor *TheGameCursor;
