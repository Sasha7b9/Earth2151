// 2025/02/22 22:59:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Game/Earth2150Tool.h"


void Earth2150::Reader::ReadListLands(pchar dir, Array<Level2150> &)
{

}


bool Earth2150::Reader::ReadLand(Level2150 &info)
{
    if (Unzipper::UnzipFile(info.file_name_lnd, info.dataLND) &&
        Unzipper::UnzipFile(info.file_name_mis, info.mis))
    {
        File file_lnd;

        if (file_lnd.OpenFile(info.file_name_lnd + ".bin", kFileCreate) == kFileOkay)
        {
            file_lnd.WriteFile(info.dataLND.data, info.dataLND.pointer);
        }

        File file_mis;

        if (file_mis.OpenFile(info.file_name_mis + ".bin", kFileCreate) == kFileOkay)
        {
            file_mis.WriteFile(info.mis.data, info.mis.pointer);
        }

        return true;
    }

    LOG_ERROR_HI("Can not read level %s", info.file_name_lnd.c_str());

    return false;
}
