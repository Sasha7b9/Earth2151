// 2025/03/05 11:09:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Earth2150/Parameters/Models/Abstracts/TypedEntity.h"
#include "Earth2150/Files/Model2150.h"


// Данные сущности обладают физическим представлением - это объекты на карте
struct InteractableEntity : public TypedEntity
{
    InteractableEntity(const String<> &name, const Array<int> &researches, EntityClassType::E type, FileReader &reader) :
        TypedEntity(name, researches, type)
    {
        mesh_name = reader.ReadString(4);
        shadow_type = reader.ReadInt();
        view_params_index = reader.ReadInt();
        cost = reader.ReadInt();
        time_build = reader.ReadInt();
        sound_pack_ID = reader.ReadString(4);
        reader.ReadUInt();
        smoke_ID = reader.ReadString(4);
        reader.ReadUInt();
        kill_explosion_ID = reader.ReadString(4);
        reader.ReadUInt();
        distributed_ID = reader.ReadString(4);
        reader.ReadUInt();
    }


    virtual void Save(int i, File &file) override
    {
        TypedEntity::Save(i, file);

        file.WriteString(Text::Format("     mesh \"%s\", shadow type %d, view params index %d, cost %d, time build %d, sound pack ID \"%s\", smoke ID \"%s\", kill explosion ID \"%s\", distributed ID \"%s\"",
            mesh_name.c_str(), shadow_type, view_params_index, cost, time_build, sound_pack_ID.c_str(), smoke_ID.c_str(), kill_explosion_ID.c_str(), distributed_ID.c_str()));
    }


    String<> mesh_name;
    int shadow_type;
    int view_params_index;
    int cost;
    int time_build;
    String<> sound_pack_ID;
    String<> smoke_ID;
    String<> kill_explosion_ID;
    String<> distributed_ID;
};
