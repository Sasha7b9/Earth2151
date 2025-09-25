// 2025/03/05 20:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/Models/Abstracts/TypelessEntity.h"


struct TalkPack : public TypelessEntity
{
    TalkPack(const String<> &name, const Array<int> &researches, FileReader &reader) :
        TypelessEntity(name, researches)
    {
        selected = reader.ReadString();
        move = reader.ReadString();
        attack = reader.ReadString();
        command = reader.ReadString();
        enemy = reader.ReadString();
        help = reader.ReadString();
        free_way = reader.ReadString();
    }

    String<> selected;
    String<> move;
    String<> attack;
    String<> command;
    String<> enemy;
    String<> help;
    String<> free_way;
};