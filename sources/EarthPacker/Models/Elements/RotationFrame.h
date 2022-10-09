// 2022/09/24 21:08:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/FileInputStream.h"


class RotationFrame : public glm::mat4
{
public:
    RotationFrame(FileInputStream &);
};
