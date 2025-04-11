// 2025/03/17 21:12:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ChoicePlayerWindow : public Pi::Window, public Global<ChoicePlayerWindow>
{
public:

    ChoicePlayerWindow();

    virtual void PreprocessWidget() override;

    void Localize();

    String<> ResolveNamePlayer();

private:

    ListWidget *listPlayers = nullptr;

    WidgetObserver<ChoicePlayerWindow> widgetObserver{ this, &ChoicePlayerWindow::HandleWidgetEvent };

    void HandleWidgetEvent(Widget *, const WidgetEventData *);

    void StartEnterGameWindowIfNeed();

    void FillListUsers();
};


extern ChoicePlayerWindow *TheChoicePlayerWindow;
