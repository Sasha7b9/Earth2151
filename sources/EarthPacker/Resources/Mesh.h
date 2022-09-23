// 2022/09/23 21:52:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Resources/Resource.h"


namespace Resources
{
    class Mesh : public Resource
    {
    public:
        Mesh(std::string &filename, ResourceInfo, wxMemoryBuffer *);
    private:
    };
}
