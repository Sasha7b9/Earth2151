// 2022/09/24 14:55:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Models/ModelPart.h"
#include "Utils/FileInputStream.h"
#include "Models/Elements/ModelTemplate.h"
#include "Models/Collections/MountPoints.h"
#include "Models/Collections/Lights.h"
#include "Models/PartNode.h"


namespace Models
{
    class Model
    {
    public:
        Model(const wxString &path);
    private:
        std::list<ModelPart *> parts;
        int type;
        ModelTemplate *model_template = nullptr;
        MountPoints *mount_points = nullptr;
        Lights *lights = nullptr;

        uint16 unknown_val1 = 0;
        uint16 unknown_val2 = 0;
        uint16 unknown_val3 = 0;
        uint16 unknown_val4 = 0;
        uint   unknown_val5 = 0;

        PartNode *partsTree = nullptr;

        void CheckHeader(FileInputStream &);

        void GetParts(FileInputStream &, std::list<ModelPart *> &);

        PartNode *GetPartsTree();
    };
}