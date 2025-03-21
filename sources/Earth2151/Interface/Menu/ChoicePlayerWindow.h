// 2025/03/17 21:12:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ChoicePlayerWindow : public Pi::Window, public Global<ChoicePlayerWindow>
{
public:

    ChoicePlayerWindow();

    virtual void PreprocessWidget() override;

private:

    ListWidget *listPlayers = nullptr;

    WidgetObserver<ChoicePlayerWindow> btnApplyObserver{ this, &ChoicePlayerWindow::HandleButtonEvent };
    WidgetObserver<ChoicePlayerWindow> btnDeleteObserver{ this, &ChoicePlayerWindow::HandleButtonEvent };
    WidgetObserver<ChoicePlayerWindow> btnBackObserver{ this, &ChoicePlayerWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern ChoicePlayerWindow *TheChoicePlayerWindow;
