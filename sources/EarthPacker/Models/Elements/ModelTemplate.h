// 2022/09/24 15:59:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


struct InfoModel;
struct DescriptionModel;


class ModelTemplate
{
public:

    void Create(FileInputStream &, DescriptionModel &);

private:

    static const int ROWS = 4;
    static const int COLUMNS = 4;

    bool matrix[ROWS][COLUMNS];
};
