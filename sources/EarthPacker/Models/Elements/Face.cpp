// 2022/09/24 20:55:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Face.h"


Face::Face(FileInputStream &stream)
{
    v1 = stream.Read2Bytes();
    v2 = stream.Read2Bytes();
    v3 = stream.Read2Bytes();
    unknown = stream.Read2Bytes();
}
