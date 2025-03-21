// 2025/03/05 16:43:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Equipment.h"
#include "Earth2151.h"


struct Repairer : public Equipment
{
    Repairer(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Equipment(name, researhces, type, reader)
    {
        if (TheEarth2151->IsMoonProject())
        {
            reader.ReadBuffer(&param, sizeof(param));
        }
        else
        {
            reader.ReadBuffer(&param, sizeof(int) * 7);
            reader.ReadBuffer(&param + sizeof(int) * 11, sizeof(Parameters) - sizeof(int) * 11);
        }
    }

    struct Parameters
    {
        int repairer_flags;
        int repair_HP_per_tick;
        int repair_electronics_per_tick;
        int ticks_per_repair;
        int convert_tank_time;
        int convert_buildiing_time;
        int convert_healthy_tank_time;

        int convert_healty_building_time;   // \ 
        int repaint_tank_time;              // | Этих нет в Earth 2150, начинаются с The Moon Project
        int repaint_building_time;          // |
        int upgrade_tank_time;              // /

        AnimLine anim_repair_start;
        AnimLine anim_repair_work;
        AnimLine anim_repair_end;
        AnimLine anim_convert_start;
        AnimLine anim_convert_work;
        AnimLine anim_convert_end;
        AnimLine anim_repaint_start;
        AnimLine anim_repaint_work;
        AnimLine anim_repaint_end;
    };

    Parameters param;
};
