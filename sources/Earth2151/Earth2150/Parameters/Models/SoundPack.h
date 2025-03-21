// 2025/03/05 16:33:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct SoundPack : public TypelessEntity
{
    SoundPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        normal_wave_pack1 = reader.ReadString(4);
        normal_wave_pack2 = reader.ReadString(4);
        normal_wave_pack3 = reader.ReadString(4);
        normal_wave_pack4 = reader.ReadString(4);

        looped_wave_pack1 = reader.ReadString(4);
        looped_wave_pack2 = reader.ReadString(4);
        looped_wave_pack3 = reader.ReadString(4);
        looped_wave_pack4 = reader.ReadString(4);
    }

    String<> normal_wave_pack1;
    String<> normal_wave_pack2;
    String<> normal_wave_pack3;
    String<> normal_wave_pack4;
    String<> looped_wave_pack1;
    String<> looped_wave_pack2;
    String<> looped_wave_pack3;
    String<> looped_wave_pack4;
};
