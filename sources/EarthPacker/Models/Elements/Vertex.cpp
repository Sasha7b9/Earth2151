// 2022/09/24 19:45:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Models/Elements/Vertex.h"


Vertex::Vertex(const Vector &_position, const Vector &_normal, float _u, float _v) :
    position(_position), normal(_normal), u(_u), v(_v)
{

}
