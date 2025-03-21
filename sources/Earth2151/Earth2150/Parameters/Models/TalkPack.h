// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypelessEntity.h"


struct TalkPack : public TypelessEntity
{
    TalkPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        selected = reader.ReadString(4);
        move = reader.ReadString(4);
        attack = reader.ReadString(4);
        command = reader.ReadString(4);
        enemy = reader.ReadString(4);
        help = reader.ReadString(4);
        free_way = reader.ReadString(4);
    }

    String<> selected;
    String<> move;
    String<> attack;
    String<> command;
    String<> enemy;
    String<> help;
    String<> free_way;
};