// 2025/04/10 16:21:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Mission
{
    static void Register(int id, pchar file_level, pchar file_script, pchar briefing, uint flags,
        int longitude, int latitude, int d1, int d2, int d3,
        int id_next1 = -1, int id_next2 = -1, int id_next3 = -1, int id_next4 = -1);

    static void Destroy();

    int id;
    String<> file_level;
    String<> file_script;
    String<> briefing;
    uint flags;
    int longitude;
    int latitude;
    int d1;
    int d2;
    int d3;
    int id_next1;
    int id_next2;
    int id_next3;
    int id_next4;

    static const Mission *Get(int id);

private:

    static std::map<int, Mission *> missions;
};
