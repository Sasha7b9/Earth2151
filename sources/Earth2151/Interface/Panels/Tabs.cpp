// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Panels/Tabs.h"
#include "Interface/Controls/Canvas.h"
#include "Interface/Panels/PanelWindow.h"
#include "Settings.h"
#include "Interface/Widgets/ObjectViewportWidget.h"
#include "Interface/Controls/CheckBox.h"


NotebookMainPanel *TheNotebookMainPanel = nullptr;
TabInfo *TheTabInfo = nullptr;
TabUnits *TheTabUnits = nullptr;
TabPlatoons *TheTabPlatoons = nullptr;
TabStructures *TheTabStructures = nullptr;


TabTitle::TabTitle(Notebook *parent, Array<Tab *> *_tabs)
    : Widget(),
    observerSwitchTab(static_cast<NotebookMainPanel *>(parent), &NotebookMainPanel::HandleSwitchTab)
{

    this->parent = parent;
    this->tabs = _tabs;

    float x = 10;
    float y = 5;
    float step = SET::GUI::BUTTON::WIDTH(TButton::Type::NameTab) - 1.0F;

    buttons.SetArrayElementCount(_tabs->GetArrayElementCount());

    for (int64 i = 0; i < _tabs->GetArrayElementCount(); i++)
    {
        buttons[i] = new TButton(TButton::Type::NameTab, { x, y }, (*_tabs)[(size_t)i]->Name());
        AppendNewSubnode(buttons[(size_t)i]);
        x += step;
        buttons[(size_t)i]->SetObserver(&observerSwitchTab);
    }
}


Tab::Tab(Notebook *parent, pchar name) : Widget()
{
    this->parent = parent;
    this->name = name;
    SetWidgetPosition(Point2D((float)SET::GUI::TAB_WIDGET::BORDER::LEFT(), (float)SET::GUI::TAB_WIDGET::BORDER::TOP()));
}


TabInfo::TabInfo(Notebook *parent, pchar name)
    : Tab(parent, name), Global<TabInfo>(TheTabInfo)
{
    //float delta = 3.0f;
    /*
    viewer = new ObjectViewportWidget({164, 164}, 1.0f);
    viewer->SetWidgetPosition({delta, delta, 0.0f});
    AppendNewSubnode(viewer);
    */
}


TabInfo::~TabInfo()
{
    //TheGameWorld->GetRootNode()->RemoveSubnode(viewer->GetViewportCamera());
    //RemoveSubnode(viewer);
    //SAFE_DELETE(viewer);
}


TabUnits::TabUnits(Notebook *parent, pchar name)
    : Tab(parent, name), Global<TabUnits>(TheTabUnits)
{

    float x = 10;
    float y = 10;
    float step = SET::GUI::CHECKBOX::HEIGHT() + 3.0f;

    TButton *button = new TButton(TButton::Type::Normal, { 10.0f, 10.0f }, _("Юниты"));
    AppendNewSubnode(button);

    chbxAll = new CheckBox(kTypeNormal, { x, y }, "All");
    AppendNewSubnode(chbxAll);

    chbxCivil = new CheckBox(kTypeNormal, { x, y = y + step }, _("Гражданские"));
    AppendNewSubnode(chbxCivil);

    chbxMilitary = new CheckBox(kTypeNormal, { x, y = y + step }, "Military");
    AppendNewSubnode(chbxMilitary);

    chbxGround = new CheckBox(kTypeNormal, { x, y = y + step }, "Ground");
    AppendNewSubnode(chbxGround);

    chbxWater = new CheckBox(kTypeNormal, { x, y = y + step }, "Water");
    AppendNewSubnode(chbxWater);

    chbxAir = new CheckBox(kTypeNormal, { x, y = y + step }, "Air");
    AppendNewSubnode(chbxAir);
}

TabUnits::~TabUnits()
{
    SAFE_DELETE(chbxAir);
    SAFE_DELETE(chbxWater);
    SAFE_DELETE(chbxGround);
    SAFE_DELETE(chbxMilitary);
    SAFE_DELETE(chbxCivil);
    SAFE_DELETE(chbxAll);
}

TabPlatoons::TabPlatoons(Notebook *parent, pchar name)
    : Tab(parent, name), Global<TabPlatoons>(TheTabPlatoons)
{

    TButton *button = new TButton(TButton::Type::Normal, { 15.0f, 15.0f }, "Platoons");
    AppendNewSubnode(button);
}


TabStructures::TabStructures(Notebook *parent, pchar name)
    : Tab(parent, name), Global<TabStructures>(TheTabStructures)
{

    TButton *button = new TButton(TButton::Type::Normal, { 20.0f, 20.0f }, "Structures");
    AppendNewSubnode(button);
}



Notebook::Notebook(PanelWindow *_parent) : Widget()
{
    parent = _parent;

    float parentWidth = parent->GetWidgetSize().x;
    float parentHeight = parent->GetWidgetSize().y;
    float left = (float)(SET::GUI::TAB_WIDGET::BORDER::LEFT());
    float right = (float)(SET::GUI::TAB_WIDGET::BORDER::RIGHT());
    float top = (float)(SET::GUI::TAB_WIDGET::BORDER::TOP());
    float bottom = (float)(SET::GUI::TAB_WIDGET::BORDER::BOTTOM());
    Vector2D size(parentWidth - left - right, parentHeight - top - bottom);
    mainWidget = new Canvas(size);
    AppendNewSubnode(mainWidget);

    mainWidget->SetColorBrush(SET::GUI::COLOR::BORDER());
    mainWidget->DrawRectangle(0, 0, mainWidget->GetWidgetSize().x - 1, mainWidget->GetWidgetSize().y - 1);
    mainWidget->SetWidgetPosition({ (float)SET::GUI::TAB_WIDGET::BORDER::LEFT(), (float)SET::GUI::TAB_WIDGET::BORDER::TOP() });
    mainWidget->EndPaint();
}

Notebook::~Notebook()
{
    SAFE_DELETE(mainWidget);
}


NotebookMainPanel::NotebookMainPanel(PanelWindow *parent)
    : Notebook(parent), Global<NotebookMainPanel>(TheNotebookMainPanel)
{
    if (ApplicationMode::IsGame())
    {
        numTabs = 4;

        tabs.InsertArrayElement(Tab_Info, new TabInfo(this, _("Информация")));
        AppendNewSubnode(TheTabInfo);

        tabs.InsertArrayElement(Tab_Structures, new TabStructures(this, _("Сооружения")));
        AppendNewSubnode(TheTabStructures);

        tabs.InsertArrayElement(Tab_Platoons, new TabPlatoons(this, _("Взводы")));
        AppendNewSubnode(TheTabPlatoons);

        tabs.InsertArrayElement(Tab_Units, new TabUnits(this, _("Подразделения")));
        AppendNewSubnode(TheTabUnits);
    }

    if (tabs.GetArrayElementCount())
    {
        tabTitle = new TabTitle(this, &tabs);
        AppendNewSubnode(tabTitle);
    }

    SetActiveTab(Tab_Info);
}


NotebookMainPanel::~NotebookMainPanel()
{
    for (int i = 0; i < tabs.GetArrayElementCount(); i++)
    {
        SAFE_DELETE(tabs[i]);
    }

    SAFE_DELETE(tabTitle);
}


void NotebookMainPanel::Destroy()
{
    SAFE_DELETE(TheNotebookMainPanel);
}


void NotebookMainPanel::HandleSwitchTab(Widget *widget, const WidgetEventData *)
{
    TButton *button = static_cast<TButton *>(widget);

    if (button == tabTitle->buttons[Tab_Info])
    {
        SetActiveTab(Tab_Info);
    }
    else if (button == tabTitle->buttons[Tab_Units])
    {
        SetActiveTab(Tab_Units);
    }
    else if (button == tabTitle->buttons[Tab_Platoons])
    {
        SetActiveTab(Tab_Platoons);
    }
    else if (button == tabTitle->buttons[Tab_Structures])
    {
        SetActiveTab(Tab_Structures);
    }
}


void NotebookMainPanel::SetActiveTab(uint tabID)
{
    for (int i = 0; i < tabs.GetArrayElementCount(); i++)
    {
        Notebook::RemoveSubnode(tabs[i]);
        tabTitle->buttons[i]->SetSelected(false);
    }

    if ((int)tabID < tabs.GetArrayElementCount())
    {
        Notebook::AppendSubnode(tabs[tabID]);
        tabTitle->buttons[tabID]->SetSelected(true);
    }
}