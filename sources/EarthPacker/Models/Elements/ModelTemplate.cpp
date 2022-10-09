// 2022/09/24 16:00:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/ModelTemplate.h"
#include "Models/Model.h"


void ModelTemplate::Create(FileInputStream &stream, DescriptionModel &desc)
{
    InfoModel info(InfoModel::Type::ModelTemplate, stream.TellI(), "Model template");

    uint16 bytes = stream.Read2Bytes();

    std::bitset<16> data{ bytes };

    for (int col = COLUMNS - 1; col >= 0; col--)
    {
        for (int row = ROWS - 1; row >= 0; row--)
        {
            bool bit = (data[col * 4 + row] == 0) ? false : true;

            matrix[row][COLUMNS - 1 - col] = bit;
        }
    }

    info.AppendBytes(&bytes, 2);

    desc.AppendInfo(info, stream);
}
