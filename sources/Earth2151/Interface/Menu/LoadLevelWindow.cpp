// 2025/03/22 21:29:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Interface/Menu/LoadLevelWindow.h"
#include "Utils/Locale.h"
#include "Earth2151.h"
#include "Editor/Editor.h"
#include "Objects/World/Landscape.h"
#include "Cameras.h"


LoadLevelWindow *TheLoadLevelWindow = nullptr;


#define LIST_LEVELS     ((ListWidget *)FindWidget("listLevels"))
#define BTN_CANCEL      ((PushButtonWidget *)FindWidget("btnCancel"))
#define BTN_LOAD        ((PushButtonWidget *)FindWidget("btnLoad"))


LoadLevelWindow::LoadLevelWindow() :
    Window("panels/LoadLevel"),
    Global<LoadLevelWindow>(TheLoadLevelWindow)
{

}


void LoadLevelWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    LIST_LEVELS->SetObserver(&widgetObserver);
    BTN_CANCEL->SetObserver(&widgetObserver);
    BTN_LOAD->SetObserver(&widgetObserver);

    Localize();

    LIST_LEVELS->AppendListItem("Temp");
    LIST_LEVELS->PurgeListItems();
    LIST_LEVELS->SetItemSpacing(17);

    Map<FileReference> map;

    TheFileMgr->BuildFileMap(RESOURCE_PATH("Levels"), &map, FileMapFilterLevels);

    FileReference *element = map.GetFirstMapElement();

    while (element)
    {
        LIST_LEVELS->AppendListItem(element->fileName);
        element = element->GetNextMapElement();
    }

    if (LIST_LEVELS->GetListItemCount())
    {
        LIST_LEVELS->SelectListItem(0);
    }

    LIST_LEVELS->InvalidateWidget();
    LIST_LEVELS->UpdateWidget();

    Localize();

    LIST_LEVELS->SelectListItem(last_selected);
}


bool LoadLevelWindow::FileMapFilterLevels(pchar name, uint flags, const void *)
{
    if (name[0] == '.')
    {
        return false;
    }

    if (flags & kFileDirectory)
    {
        return false;
    }

    int length = Text::GetTextLength(name);

    return Text::CompareTextCaseless(&name[MaxZero(length - 4)], ".lnd");
}


void LoadLevelWindow::Localize()
{
    if (!TheLoadLevelWindow)
    {
        return;
    }

    Window::PreprocessWidget();

    BTN_LOAD->SetText(_L("trLoad"));
    BTN_CANCEL->SetText(_L("trCancel"));
    ((TextWidget *)FindWidget("txtLoadLevel"))->SetText(_L("trLoadLevel"));
}


void LoadLevelWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
    if (eventData->eventType == kEventWidgetActivate)
    {
        if (widget == BTN_CANCEL)
        {
            TheInterfaceMgr->RemoveWidget(TheLoadLevelWindow);
        }
        else if(widget == BTN_LOAD || widget == LIST_LEVELS)
        {
            TextWidget *text = (TextWidget *)LIST_LEVELS->GetFirstSelectedListItem();

            last_selected = LIST_LEVELS->GetFirstSelectedIndex();

            TheEditor->LoadLevel(text->GetText());

            TheInterfaceMgr->RemoveWidget(TheLoadLevelWindow);

            TheCameraRTS->SetPointFocus({ LANDSCAPE->GetNumColumns() / 2.0f, LANDSCAPE->GetNumRows() / 2.0f, 0.0f });
        }
    }
}


#undef BTN_LOAD
