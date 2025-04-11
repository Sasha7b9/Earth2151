// 2025/04/06 16:28:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ConfirmRemovePlayerWindow : public Pi::Window, public Global<ConfirmRemovePlayerWindow>
{
public:

    ConfirmRemovePlayerWindow();

    virtual void PreprocessWidget() override;

    void Localize();

private:

    WidgetObserver <ConfirmRemovePlayerWindow> widgetObserver{ this, &ConfirmRemovePlayerWindow::HandleWidgetEvent };

    void HandleWidgetEvent(Widget *, const WidgetEventData *);
};


extern ConfirmRemovePlayerWindow *TheConfirmDeletePlayerWindow;
