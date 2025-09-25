// 2025/03/05 16:33:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/TypelessEntity.h"


struct SoundPack : public TypelessEntity
{
    SoundPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        normal_wave_pack1 = reader.ReadString();
        normal_wave_pack2 = reader.ReadString();
        normal_wave_pack3 = reader.ReadString();
        normal_wave_pack4 = reader.ReadString();

        looped_wave_pack1 = reader.ReadString();
        looped_wave_pack2 = reader.ReadString();
        looped_wave_pack3 = reader.ReadString();
        looped_wave_pack4 = reader.ReadString();
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
