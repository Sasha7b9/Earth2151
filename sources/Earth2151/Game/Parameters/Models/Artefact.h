// 2025/03/05 17:40:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/PassiveEntity.h"


struct Artefact : public PassiveEntity
{
    Artefact(const String<> &name, const Array<int> &researhces, EntityClassType::E _type, FileReader &reader) :
        PassiveEntity(name, researhces, _type, reader)
    {
        reader.ReadBuffer(&param, sizeof(param));
    }

    struct Parameters
    {
        int mask;
        int param;
        int respawn_time;
    };

    Parameters param;
};
