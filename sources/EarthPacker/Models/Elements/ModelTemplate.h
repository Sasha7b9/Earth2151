// 2022/09/24 15:59:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"
#include "Utils/Matrix.h"


namespace Models
{
    class ModelTemplate
    {
    public:
        ModelTemplate(FileInputStream &);
    private:
        static const int ROWS = 4;
        static const int COLUMNS = 4;

        bool matrix[ROWS][COLUMNS];
    };
}