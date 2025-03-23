// 2025/03/17 22:44:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class EnterGameWindow : public Pi::Window, public Global<EnterGameWindow>
{
public:

    EnterGameWindow();

    virtual void PreprocessWidget() override;

    void Localize();

private:

    WidgetObserver<EnterGameWindow> btnNewGameObserver{ this, &EnterGameWindow::HandleButtonEvent };
    WidgetObserver<EnterGameWindow> btnLoadGameObserver{ this, &EnterGameWindow::HandleButtonEvent };
    WidgetObserver<EnterGameWindow> btnTutorialObserver{ this, &EnterGameWindow::HandleButtonEvent };
    WidgetObserver<EnterGameWindow> btnVideoObserver{ this, &EnterGameWindow::HandleButtonEvent };
    WidgetObserver<EnterGameWindow> btnBackObserver{ this, &EnterGameWindow::HandleButtonEvent };
    WidgetObserver<EnterGameWindow> btnOuitObserver{ this, &EnterGameWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern EnterGameWindow *TheEnterGameWindow;
