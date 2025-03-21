// 2025/03/05 17:24:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Equipment.h"


struct OmnidirectionalEquipment : public Equipment
{
    OmnidirectionalEquipment(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Equipment(name, researhces, type, reader)
    {
        reader.ReadBuffer(&param, sizeof(param));
    }

    struct Parameters
    {
        int look_round_type_mask;
        int look_round_range;
        int turn_speed;
        int banner_add_experience_level;
        int regeneratioin_HP_miltiple;
        int shiedl_reload_add;
    };

    Parameters param;
};
