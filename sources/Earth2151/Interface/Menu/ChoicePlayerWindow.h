// 2025/03/17 21:12:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ChoicePlayerWindow : public Window, public Global<ChoicePlayerWindow>
{
public:

    ChoicePlayerWindow();

    virtual void PreprocessWidget() override;

    void Localize();

private:

    ListWidget *listPlayers = nullptr;

    WidgetObserver<ChoicePlayerWindow> btnObserver{ this, &ChoicePlayerWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern ChoicePlayerWindow *TheChoicePlayerWindow;
