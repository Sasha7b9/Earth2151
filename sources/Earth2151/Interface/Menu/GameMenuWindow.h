// 2025/03/18 08:10:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class GameMenuWindow : public Pi::Window, public Global<GameMenuWindow>
{
public:

    GameMenuWindow();

    virtual void PreprocessWidget() override;

    virtual bool HandleKeyboardEvent(const KeyboardEventData *) override;

    void Localize();

private:

    WidgetObserver<GameMenuWindow> btnBackToGameObserver{ this, &GameMenuWindow::HandleButtonEvent };
    WidgetObserver<GameMenuWindow> btnExitGameObserver{ this, &GameMenuWindow::HandleButtonEvent };

    void HandleButtonEvent(Widget *, const WidgetEventData *);
};


extern GameMenuWindow *TheGameMenuWindow;