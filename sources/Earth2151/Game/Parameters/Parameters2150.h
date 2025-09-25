// 2025/03/05 08:50:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Game/Parameters/ParFile.h"
#include "Game/Files/InfoHeader.h"


class Parameters2150
{
public:

    Parameters2150(pchar file_name);

    ~Parameters2150()
    {
        Destroy();
    }

    void Destroy();

    void Save();

    Entity *GetEntity(pchar);

    uint64 time_find_us = 0;      // Время поиска 

private:

    InfoHeader header;
    Array<EntityGroup> groups;
    Array<Research> researches;

    bool ReadGroups(FileReader &);
    void ReadResearch(FileReader &);

    String<> file_name;
};
