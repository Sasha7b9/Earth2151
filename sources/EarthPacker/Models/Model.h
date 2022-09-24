// 2022/09/24 14:55:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/ModelPart.h"
#include "Utils/FileInputStream.h"


namespace Models
{
    class Model
    {
    public:
        Model(const wxString &path);
    private:
        std::string file_path;
        std::list<ModelPart> parts;
        int type;

        void CheckHeader(FileInputStream &);
    };
}