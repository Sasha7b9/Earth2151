// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Panels/PanelWindow.h"
#include "Interface/Controls/Button.h"
#include "Interface/Widgets/ObjectViewportWidget.h"
#include "Interface/Controls/CheckBox.h"
#include "Settings.h"


class Tab;
class Notebook;
class NotebookMainPanel;


class TabTitle : public Widget
{
    friend class NotebookMainPanel;
public:
    TabTitle(Notebook *parent, Array<Tab *> *tabs);
    virtual ~TabTitle() { };

    void MoveWidget() override
    {
    };

private:
    TabTitle();
    Notebook           *parent;
    Array<Tab *>       *tabs;
    Array<TButton *>    buttons;
    WidgetObserver<NotebookMainPanel> observerSwitchTab;
};

class Notebook : public Widget
{

public:
    Notebook(PanelWindow *parent);
    ~Notebook();

    void MoveWidget() override
    {
    };
    void AddTab(Tab *tab);

protected:
    TabTitle    *tabTitle = nullptr;
    Canvas      *mainWidget = nullptr;
    int numTabs = 0;
    Array<Tab *> tabs;

private:
    Notebook();
    PanelWindow *parent = nullptr;
};


class NotebookMainPanel : public Notebook, public Global<NotebookMainPanel>
{
    enum
    {
        Tab_Info,
        Tab_Structures,
        Tab_Platoons,
        Tab_Units
    };
public:
    NotebookMainPanel(PanelWindow *parent);
    ~NotebookMainPanel();

    static void Destroy();

    void HandleSwitchTab(Widget *, const WidgetEventData *);
    void SetActiveTab(uint tabID);
};


// Базовый класс для вкладок на главной панели
class Tab : public Widget
{
    friend class TabTitle;
    friend class NotebookMainPanel;
public:
    Tab();
    Tab(Notebook *parent, pchar name);
    virtual ~Tab()
    {
    };

    virtual void MoveWidget() override
    {
    };
    pchar Name()
    {
        return name;
    };

protected:
    Notebook *parent = nullptr;
    pchar name = nullptr;
};


// Главная панель - "Информация"
class TabInfo : public Tab, public Global<TabInfo>
{
public:
    TabInfo(Notebook *parent, pchar name);
    virtual ~TabInfo();

private:
    TabInfo();

    ObjectViewportWidget *viewer = nullptr;
};


// Главная панель - "Сооружения"
class TabStructures : public Tab, public Global<TabStructures>
{
public:
    TabStructures(Notebook *parent, pchar name);
    virtual ~TabStructures() { };

private:
    TabStructures();
};


// Главная панель - "Взводы"
class TabPlatoons : public Tab, public Global<TabPlatoons>
{
public:
    TabPlatoons(Notebook *parent, pchar name);
    virtual ~TabPlatoons() { };

private:
    TabPlatoons();
};


// Главная панель - "Подразделения"
class TabUnits : public Tab, public Global<TabUnits>
{
public:
    TabUnits(Notebook *parent, pchar name);
    ~TabUnits() override;

private:
    TabUnits();

    CheckBox *chbxAll = nullptr;
    CheckBox *chbxCivil = nullptr;
    CheckBox *chbxMilitary = nullptr;
    CheckBox *chbxGround = nullptr;
    CheckBox *chbxWater = nullptr;
    CheckBox *chbxAir = nullptr;
};


extern NotebookMainPanel *TheNotebookMainPanel;
extern TabInfo            *TheTabInfo;
extern TabUnits           *TheTabUnits;
extern TabPlatoons        *TheTabPlatoons;
extern TabStructures      *TheTabStructures;
