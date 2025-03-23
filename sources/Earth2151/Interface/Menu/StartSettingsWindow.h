// 2025/03/22 16:06:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class StartSettingsWindow : public Window, public Global<StartSettingsWindow>
{
public:

    StartSettingsWindow();

    virtual void PreprocessWidget() override;

    void Localize();

private:

    WidgetObserver<StartSettingsWindow> btnObserver{ this, &StartSettingsWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern StartSettingsWindow *TheStartSettingsWindow;
