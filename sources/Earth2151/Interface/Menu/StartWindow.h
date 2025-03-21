// 2024/09/20 09:46:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


using namespace Pi;


class StartWindow : public Pi::Window, public Global<StartWindow>
{
public:

    StartWindow();

    ~StartWindow() { };

    virtual void PreprocessWidget() override;

private:

    WidgetObserver<StartWindow> buttonEDObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonUCSObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonLCObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonNetworkObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonSkirmishObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonEditorObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonAboutObserver{ this, &StartWindow::HandleButtonEvent };
    WidgetObserver<StartWindow> buttonQuitObserver{ this, &StartWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern StartWindow *TheStartWindow;
