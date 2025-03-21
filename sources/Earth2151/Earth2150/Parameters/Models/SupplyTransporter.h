// 2025/03/05 13:10:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SupplyTransporter : public Vehicle
{
    SupplyTransporter(const String<> &name, const Array<int> &researhces, EntityClassType::E type, FileReader &reader) :
        Vehicle(name, researhces, type, reader)
    {
        ammoe_capacity = reader.ReadInt();
        anim_down.Read(reader);
        anim_up.Read(reader);
    }

    int ammoe_capacity;

    AnimLine anim_down;
    AnimLine anim_up;
};
