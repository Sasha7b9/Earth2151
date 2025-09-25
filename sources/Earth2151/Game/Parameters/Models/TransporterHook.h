// 2025/03/21 10:10:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Equipment.h"
#include "Earth2151.h"


struct TransporterHook : public Equipment
{
    TransporterHook(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Equipment(name, researhces, type, reader)
    {
        reader.ReadBuffer(&param, sizeof(param));
    }

    struct Parameters
    {
        AnimLine anim_down;
        AnimLine anim_up;
        int angle_get_put;
        int angel_get_unit_by_land_transporter;
        int take_height;
    };

    Parameters param;
};
