// 2022/09/24 16:00:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/ModelTemplate.h"
#include "Models/Model.h"


void ModelTemplate::Create(FileInputStream &stream)
{
    info = new InfoModel();
    info->name = "Model template";
    info->address = (int)stream.TellI();

    std::bitset<16> data{ stream.Read2Bytes() };

    for (int col = COLUMNS - 1; col >= 0; col--)
    {
        for (int row = ROWS - 1; row >= 0; row--)
        {
            bool bit = (data[col * 4 + row] == 0) ? false : true;

            matrix[row][COLUMNS - 1 - col] = bit;
        }
    }

    info->size = (int)stream.TellI() - info->address;
}


InfoModel &ModelTemplate::GetInfo()
{
    return *info;
}
