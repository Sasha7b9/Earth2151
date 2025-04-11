// 2024/09/20 09:46:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class StartWindow : public Pi::Window, public Global<StartWindow>
{
public:

    StartWindow();

    ~StartWindow() { };

    virtual void PreprocessWidget() override;

    void Localize();

private:

    WidgetObserver<StartWindow> buttonObserver{ this, &StartWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern StartWindow *TheStartWindow;
