// 2025/03/22 21:28:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class LoadLevelWindow : public Pi::Window, public Global<LoadLevelWindow>
{
public:

    LoadLevelWindow();

    virtual void PreprocessWidget() override;

    void Localize();

private:

    WidgetObserver<LoadLevelWindow> widgetObserver{ this, &LoadLevelWindow::HandleWidgetEvent };

    void HandleWidgetEvent(Widget *, const WidgetEventData *);

    static bool FileMapFilterLevels(pchar name, uint flags, const void *);
};


extern LoadLevelWindow *TheLoadLevelWindow;
