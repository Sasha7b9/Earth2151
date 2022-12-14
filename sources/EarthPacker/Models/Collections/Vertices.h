// 2022/09/24 19:44:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/Elements/Vertex.h"


class Vertices : public std::list<Vertex>, public IInputStream
{
public:
    void Create();
private:
    static const int VERTICES_BLOCK_LENGTH = 160;
    static const int VERTICES_IN_BLOCK = 4;
    static const int FIELD_SIZE = 4;
    void GetVertices(int num_block);
    float TakeFloat(const void *);
};
