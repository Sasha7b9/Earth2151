// 2025/03/05 17:20:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ContainerTransporter : public Equipment
{
    ContainerTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Equipment(name, researhces, type, reader)
    {
        reader.ReadBuffer(&param, sizeof(param));
    }

    struct Parameters
    {
        AnimLine anim_down;
        AnimLine anim_up;
    };

    Parameters param;
};
