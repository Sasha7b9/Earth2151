// 2022/09/23 20:33:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Resources/Resource.h"


namespace ResourceFactory
{
    enum class TypeResource
    {
        _0         = 0,
        Generic    = 1,
        Player     = 3,
        Text       = 5,
        Interface  = 9,     // ��� ��� ��������
        Dialog     = 17,
        Interface2 = 25,    // ��� ��� ��������
        _33        = 33,
        Level      = 43,    // ��� ��� ��������
        Mesh       = 49,
        _57        = 57,
        Level2     = 59,
        Group      = 255
    };

    Resource Create(wxInputStream &);
};
