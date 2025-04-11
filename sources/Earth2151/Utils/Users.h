// 2025/04/06 01:13:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Users
{
    // Первое имя
    String<> GetFirst();

    // Следующее имя
    String<> GetNext();

    // true, если есть игрок с таким именем
    bool Exist(const String<> &);

    // Возвращает false, если игрок с таким именем уже существует
    bool Create(const String<> &);

    bool Remove(const String<> &);

    // Возвращает false, если такого игрока нет
    bool Enter(const String<> &);
}
