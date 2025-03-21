// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "PanelMain.h"
#include "Settings.h"
#include "Interface/Panels/Tabs.h"


PanelMain *ThePanelMain = nullptr;


PanelMain::PanelMain()
    : PanelWindow(SET::GUI::MAIN::SIZE()),
    Global<PanelMain>(ThePanelMain)
{

    SetMovementMutator(Point2D((float)SET::GUI::MAIN::VIEW::X(), (float)SET::GUI::MAIN::VIEW::Y()),
        Point2D((float)SET::GUI::MAIN::VIEW::X(), (float)SET::WINDOW::SIZE().y),
        Vector2D(0.0f, SET::GUI::PANEL::SPEED()));

    AppendNewSubnode(new NotebookMainPanel(this));
}

PanelMain::~PanelMain()
{
    delete TheNotebookMainPanel;
}


void PanelMain::HandleHideShow(Widget *, const WidgetEventData *)
{
    Toggle();
}