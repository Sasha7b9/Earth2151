// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Controls/Canvas.h"


Frame *Frame::self = nullptr;


enum
{
    TOOL_OPEN,
    TOOL_SAVE,
    TOOL_NEW,

    TOOL_UNDO,
    TOOL_REDO,

    TOOL_VIEW_BRIEF,        // Сокращённый вид отображения
    TOOL_VIEW_FULL,         // Полный вид отображения

    MEAS_PRESSURE,          // Давление
    MEAS_ILLUMINATION,      // Освещённость
    MEAS_HUMIDITY,          // Влажность
    MEAS_VELOCITY,          // Скорость
    MEAS_TEMPERATURE        // Температура
};


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, _("Файл"));

    wxMenu *menuSettings = new wxMenu;
    menuBar->Append(menuSettings, _("Настройки"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    menuBar->Append(menuHelp, _("Помощь"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_MENU, &Frame::OnViewBrief, this, TOOL_VIEW_BRIEF);
    Bind(wxEVT_MENU, &Frame::OnViewFull, this, TOOL_VIEW_FULL);

    Bind(wxEVT_SIZE, &Frame::OnSize, this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    notebookLeft = new wxNotebook(this, wxID_ANY);

    cntrlDir = new ControlDir(notebookLeft);

    Bind(wxEVT_DIRCTRL_FILEACTIVATED, &Frame::OnFileActivated, this);

    notebookLeft->AddPage(cntrlDir, "Directory");

    sizer->Add(notebookLeft);

    sizer->Add(new Canvas(this));

    SetSizer(sizer);

    SetClientSize(1024, 600);
    wxWindowBase::SetMinClientSize({ 800, 300 });
}


void Frame::AddTool(int id, const wxString &label, pchar nameResource, pchar nameResourceDisabled)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

    wxBitmap bitmapDisabled(nameResourceDisabled ? wxBitmap(nameResourceDisabled, wxBITMAP_TYPE_BMP_RESOURCE) : bitmap);

    toolBar->AddTool(id, label, bitmap, bitmapDisabled, wxITEM_NORMAL, label, label);
}


void Frame::OnFileActivated(wxTreeEvent &event)
{
    wxFileName fileName(cntrlDir->GetPath(event.GetItem()));

    if (fileName.GetExt() != "wd")
    {
        return;
    }

    wxFileInputStream stream(fileName.GetFullPath());

    if (!IsValidWDFile(stream))
    {
        return;
    }

    size_t stream_size = stream.GetSize();

    uint8 *data = new uint8[stream.GetSize()];

    stream.ReadAll(data, stream.GetSize());

    uint dirLn = 0;

    stream_size = stream.GetSize();

    std::memcpy(&dirLn, data + stream.GetSize() - 30000, 4);

    dirLn = dirLn;

    delete []data;
}


bool Frame::IsValidWDFile(wxFileInputStream &stream)
{
    wxZlibInputStream zstream(stream);

    uint8 buffer[8];
    static const uint8 template_buffer[8] = { 0xff, 0xa1, 0xd0, '1', 'W', 'D', 0x00, 0x02 };

    zstream.Read(buffer, 8);

    return std::memcmp(buffer, template_buffer, 8) == 0;
}


void Frame::OnViewBrief(wxCommandEvent &)
{

}


void Frame::OnViewFull(wxCommandEvent &)
{

}


void Frame::OnMeasurePressure(wxCommandEvent &)
{

}


void Frame::OnMeasureIllumination(wxCommandEvent &)
{

}


void Frame::OnMeasureHumidity(wxCommandEvent &)
{

}


void Frame::OnMeasureVelocity(wxCommandEvent &)
{

}


void Frame::OnMeasureTemperature(wxCommandEvent &)
{

}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    self = nullptr;

    Close(true);

    OnClose();
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    self = nullptr;

    event.Skip();

    OnClose();
}


void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void Frame::OnSize(wxSizeEvent &event)
{
    Canvas::self->SetSizeArea(GetClientRect().width - ControlDir::self->GetSize().x, GetClientRect().height);

    wxSize size = { ControlDir::self->GetSize().GetWidth(), GetClientRect().height };

    ControlDir::self->SetMinClientSize(size);
    ControlDir::self->SetClientSize(size);
    ControlDir::self->SetSize(size);

    event.Skip();
}
