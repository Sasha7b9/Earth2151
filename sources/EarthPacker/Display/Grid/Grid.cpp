// 2022/09/05 10:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid/Grid.h"
#include "Data/Sensors.h"


using namespace std;


Grid *Grid::self = nullptr;


Grid *Grid::Create(wxWindow *parent, const wxSize &size)
{
    self = new Grid(parent, size);

    return self;
}


Grid::Grid(wxWindow *parent, const wxSize &size) :
    wxGrid(parent, wxID_ANY, { 0, 0 }, size)
{
    CreateGrid(0, 0);

    AppendCols(6);

    EnableEditing(false);

    DisableCellEditControl();

    SetRowLabelSize(0);

    SetColLabelValue(0, "ID");

    for (int meas = 0; meas < TypeMeasure::Count; meas++)
    {
        SetColLabelValue(meas + 1, wxString(TypeMeasure::GetTitle((TypeMeasure::E)meas)) +
            wxString("\n") + wxString(TypeMeasure::GetUnits((TypeMeasure::E)meas)));
    }

    SetColSize(0, FromDIP(40));

    for (int i = 1; i < 5; i++)
    {
        SetColSize(i, FromDIP(60));
    }

//    wxScrollHelperBase::SetScrollbars(20, 20, 5, 5);
}


void Grid::SetMeasure(uint id, uint8 type, float value)
{
    auto row = rows.find(id);

    if (row == rows.end()) 
    {
        AppendRows(1);

        rows.emplace(pair<uint, int>(id, GetNumberRows() - 1));

        SetCellValue(GetNumberRows() - 1, 0, wxString::Format("%d", id));
    }

    row = rows.find(id);

    SetCellValue(row->second, type + 1, wxString::Format("%10.2f", value));
}
