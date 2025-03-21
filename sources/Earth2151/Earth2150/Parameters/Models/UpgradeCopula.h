// 2025/03/05 17:29:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Equipment.h"


struct UpgradeCopula : public Equipment
{
    UpgradeCopula(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Equipment(name, researhces, type, reader)
    {

    }
};
