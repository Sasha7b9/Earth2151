// 2025/02/14 23:05:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Mouse;
class TButton;
class PanelMap;

class MouseObservable : public Observable<MouseObservable, Type>
{
public:
    MouseObservable()
    {
    };
    virtual ~MouseObservable();
private:
};


class Mouse : public Global<Mouse>
{
public:

    enum class Event
    {
        LeftChanged,
        MiddleChanged,
        RightChanged,
        PositionChanged
    };

    Mouse();

    void AddObserver(Observer<TButton, MouseObservable> *observer);
    void AddObserver(Observer<PanelMap, MouseObservable> *observer);

    void ChangePos(int posX, int posY)
    {
        leftNowPressed = false;
        leftNowReleased = false;
        pos.x = (float)posX;
        pos.y = (float)posY;
        observable.PostEvent((int)Event::LeftChanged);
        observable.PostEvent((int)Event::PositionChanged);
    }

    bool LeftIsPressed()
    {
        return leftIsPressed;
    }

    bool LeftNowReleased()
    {
        return leftNowReleased;
    }

    void SetLeftPressed();

    void SetLeftReleased();

    void SetMiddlePressed()
    {
        middleIsPressed = true;
        observable.PostEvent((int)Event::MiddleChanged);
    }

    bool MiddleIsPressed()
    {
        return middleIsPressed;
    }

    void SetMiddleReleased()
    {
        middleIsPressed = false;
        observable.PostEvent((int)Event::MiddleChanged);
    }

    bool RightIsPressed()
    {
        return rightIsPressed;
    }

    void SetRightPressed();

    void SetRightReleased();

    Point2D GetPosition()
    {
        return pos;
    }

private:

    MouseObservable observable;
    Point2D pos;

    bool leftNowPressed = false;
    bool leftNowReleased = false;
    bool leftIsPressed = false;

    bool rightNowPressed = false;
    bool rightNowReleased = false;
    bool rightIsPressed = false;

    bool middleNowPressed = false;
    bool middleNowReleased = false;
    bool middleIsPressed = false;

    Matrix4D transform_camera_begin_right_down;         // Матрица трансформации камеры при нажатии правой кнопки мыши
};


extern Mouse *TheMouse;
