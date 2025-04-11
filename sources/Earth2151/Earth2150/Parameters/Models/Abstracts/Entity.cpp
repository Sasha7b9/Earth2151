// 2025/03/05 10:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Earth2150/Parameters/Models/Builder.h"
#include "Earth2150/Parameters/Models/Harvester.h"
#include "Earth2150/Parameters/Models/Sapper.h"
#include "Earth2150/Parameters/Models/SupplyTransporter.h"
#include "Earth2150/Parameters/Models/BuildingTransporter.h"
#include "Earth2150/Parameters/Models/ResourceTransporter.h"
#include "Earth2150/Parameters/Models/UnitTransporter.h"
#include "Earth2150/Parameters/Models/Building.h"
#include "Earth2150/Parameters/Models/Weapon.h"
#include "Earth2150/Parameters/Models/Missile.h"
#include "Earth2150/Parameters/Models/SoundPack.h"
#include "Earth2150/Parameters/Models/Repairer.h"
#include "Earth2150/Parameters/Models/ContainerTransporter.h"
#include "Earth2150/Parameters/Models/OmnidirectionalEquipment.h"
#include "Earth2150/Parameters/Models/UpgradeCopula.h"
#include "Earth2150/Parameters/Models/Passive.h"
#include "Earth2150/Parameters/Models/Artefact.h"
#include "Earth2150/Parameters/Models/StartingPosition.h"
#include "Earth2150/Parameters/Models/MultiExplosion.h"
#include "Earth2150/Parameters/Models/Explosion.h"
#include "Earth2150/Parameters/Models/Smoke.h"
#include "Earth2150/Parameters/Models/FlyingWaste.h"
#include "Earth2150/Parameters/Models/Mine.h"
#include "Earth2150/Parameters/Models/WallLaser.h"
#include "Earth2150/Parameters/Models/BuilderLine.h"
#include "Earth2150/Parameters/Models/Platoon.h"
#include "Earth2150/Parameters/Models/ShieldGenerator.h"
#include "Earth2150/Parameters/Models/TransporterHook.h"
#include "Earth2150/Parameters/Models/TalkPack.h"
#include "Earth2150/Parameters/Models/Parameter.h"
#include "Earth2150/Parameters/Models/PlayerTalkPack.h"
#include "Earth2150/Parameters/Models/SpecialUpdateLink.h"
#include "Objects/World/Landscape.h"
#include "Graphics/Primitives.h"
#include "Earth2151.h"


Entity *Entity::Create(FileReader &reader, EntityGroupType::E group_type)
{
    char buffer[64];

    int p = reader.pointer;

    reader.ReadBuffer(buffer, 64);

    reader.pointer = p;


    String<> name = reader.ReadString(4);

    int num_research = reader.ReadInt();

    Array<int> researches;

    for (int i = 0; i < num_research; i++)
    {
        researches.AppendArrayElement(reader.ReadInt());
    }

    int num_field_types = reader.ReadInt();

    Array<bool> field_types;

    for (int i = 0; i < num_field_types; i++)
    {
        field_types.AppendArrayElement(reader.ReadBool());
    }

    EntityClassType::E type = EntityClassType::_None;

    switch (group_type)
    {
    case EntityGroupType::SoundPack:
    case EntityGroupType::ShieldGenerator:
    case EntityGroupType::Parameter:
    case EntityGroupType::SpecialUpdatesLink:
        break;
    default:
        type = (EntityClassType::E)reader.ReadUInt();
        break;
    }

    if (type == EntityClassType::Artefact)                      return new Artefact(name, researches, type, reader);
    else if (type == EntityClassType::Builder)                  return new Builder(name, researches, type, reader);
    else if (type == EntityClassType::BuildPassive)             return new Passive(name, researches, type, reader);
    else if (type == EntityClassType::BuilderLine)              return new BuilderLine(name, researches, type, reader);
    else if (type == EntityClassType::BuildingTransporter)      return new BuildingTransporter(name, researches, type, reader);
    else if (type == EntityClassType::Building)                 return new Building(name, researches, type, reader);
    else if (type == EntityClassType::Cannon)                   return new Weapon(name, researches, type, reader);
    else if (type == EntityClassType::ContainerTransporter)     return new ContainerTransporter(name, researches, type, reader);
    else if (type == EntityClassType::Equipment)                return new Equipment(name, researches, type, reader);
    else if (type == EntityClassType::Explosion)                return new Explosion(name, researches, type, reader);
    else if (type == EntityClassType::ExplosionEx)              return new Explosion(name, researches, type, reader);
    else if (type == EntityClassType::FlyingWaste)              return new FlyingWaste(name, researches, type, reader);
    else if (type == EntityClassType::Harvester)                return new Harvester(name, researches, type, reader);
    else if (type == EntityClassType::Mine)                     return new Mine(name, researches, type, reader);
    else if (type == EntityClassType::Missile)                  return new Missile(name, researches, type, reader);
    else if (type == EntityClassType::MultiExplosion)           return new MultiExplosion(name, researches, type, reader);
    else if (type == EntityClassType::OmnidirectionalEquipment) return new OmnidirectionalEquipment(name, researches, type, reader);
    else if (type == EntityClassType::Passive)                  return new Passive(name, researches, type, reader);
    else if (type == EntityClassType::Platoon)                  return new Platoon(name, researches, type, reader);
    else if (type == EntityClassType::Repairer)                 return new Repairer(name, researches, type, reader);
    else if (type == EntityClassType::ResourceTransporter)      return new ResourceTransporter(name, researches, type, reader);
    else if (type == EntityClassType::Sapper)                   return new Sapper(name, researches, type, reader);
    else if (type == EntityClassType::Smoke)                    return new Smoke(name, researches, type, reader);
    else if (type == EntityClassType::StartingPositionMark)     return new StartingPosition(name, researches, type, reader);
    else if (type == EntityClassType::SupplyTransporter)        return new SupplyTransporter(name, researches, type, reader);
    else if (type == EntityClassType::TransientPassive)         return new Passive(name, researches, type, reader);
    else if (type == EntityClassType::TransporterHook)          return new TransporterHook(name, researches, type, reader);
    else if (type == EntityClassType::Vehicle)                  return new Vehicle(name, researches, type, reader);
    else if (type == EntityClassType::UnitTransporter)          return new UnitTransporter(name, researches, type, reader);
    else if (type == EntityClassType::UpgradeCopula)            return new UpgradeCopula(name, researches, type, reader);
    else if (type == EntityClassType::WallLaser)                return new WallLaser(name, researches, type, reader);

    if (group_type == EntityGroupType::SoundPack)
    {
        if (SU::BeginWith(name.c_str(), "TALK_"))
        {
            return new TalkPack(name, researches, reader);
        }
        else if (SU::BeginWith(name.c_str(), "PLAYERTALK_"))
        {
            return new PlayerTalkPack(name, researches, reader);
        }
        else
        {
            return new SoundPack(name, researches, reader);
        }
    }
    else if (group_type == EntityGroupType::ShieldGenerator)
    {
        return new ShieldGenerator(name, researches, reader);
    }
    else if (group_type == EntityGroupType::Parameter)
    {
        return new Parameter(name, researches, reader, field_types);
    }
    else if (group_type == EntityGroupType::SpecialUpdatesLink)
    {
        return new SpecialUpdateLink(name, researches, reader);
    }

    LOG_ERROR("Unknown entity type");

    return nullptr;
}


void AnimLine::Read(FileReader &reader)
{
    start = reader.ReadInt();
    end = reader.ReadInt();
}


void Entity::Save(int i, File &file)
{
    file.WriteString(Text::Format("Entity %3d : %s     %s", i, name.c_str(), EntityClassType::ToString(type)));
}


pchar EntityClassType::ToString(E e)
{
    switch (e)
    {
    case EntityClassType::_None:                    return "None";
    case EntityClassType::Vehicle:                  return "Vehicle";
    case EntityClassType::SupplyTransporter:        return "SupplyTransporter";
    case EntityClassType::Builder:                  return "Builder";
    case EntityClassType::Harvester:                return "Harvester";
    case EntityClassType::Sapper:                   return "Sapper";
    case EntityClassType::Equipment:                return "Equipment";
    case EntityClassType::Cannon:                   return "Cannon";
    case EntityClassType::Repairer:                 return "Repairer";
    case EntityClassType::ContainerTransporter:     return "ContainerTransporter";
    case EntityClassType::OmnidirectionalEquipment: return "OmnidirectionalEquipment";
    case EntityClassType::UpgradeCopula:            return "UpgradeCopula";
    case EntityClassType::TransporterHook:          return "TransporterHook";
    case EntityClassType::Passive:                  return "Passive";
    case EntityClassType::Mine:                     return "Mine";
    case EntityClassType::MultiExplosion:           return "MultiExplosion";
    case EntityClassType::Building:                 return "Building";
    case EntityClassType::BuildPassive:             return "BuildPassive";
    case EntityClassType::Missile:                  return "Missile";
    case EntityClassType::Platoon:                  return "Platoon";
    case EntityClassType::TransientPassive:         return "TransientPassive";
    case EntityClassType::Artefact:                 return "Artefact";
    case EntityClassType::Explosion:                return "Explosion";
    case EntityClassType::ExplosionEx:              return "ExplosionEx";
    case EntityClassType::FlyingWaste:              return "FlyingWaste";
    case EntityClassType::StartingPositionMark:     return "StartingPositionMark";
    case EntityClassType::Smoke:                    return "Smoke";
    case EntityClassType::WallLaser:                return "WallLaser";
    case EntityClassType::BuilderLine:              return "BuilderLine";
    case EntityClassType::BuildingTransporter:      return "BuildingTransporter";
    case EntityClassType::ResourceTransporter:      return "ResourceTransporter";
    case EntityClassType::UnitTransporter:          return "UnitTransporter";
    default:
        break;
    }

    return "";
}


pchar Faction::ToString()
{
    switch (value)
    {
    case Faction::Neutral:  return "Neutral";
    case Faction::UCS:      return "UCS";
    case Faction::ED:       return "ED";
    case Faction::LC:       return "LC";
    }

    return "";
}


pchar EntityGroupType::ToString()
{
    switch (value)
    {
    case EntityGroupType::Unknown:              return "Unknown";
    case EntityGroupType::Vehicle:              return "Vehicle";
    case EntityGroupType::Cannon:               return "Cannon";
    case EntityGroupType::Missile:              return "Missile";
    case EntityGroupType::Building:             return "Building";
    case EntityGroupType::Special:              return "Special";
    case EntityGroupType::Equipment:            return "Equipment";
    case EntityGroupType::ShieldGenerator:      return "ShieldGenerator";
    case EntityGroupType::SoundPack:            return "SoundPack";
    case EntityGroupType::SpecialUpdatesLink:   return "SpecialUpdatesLink";
    case EntityGroupType::Parameter:            return "Parameter";
    }

    return "";
}


Entity *EntityGroup::GetEntity(pchar name)
{
    for (Entity *entity : entities)
    {
        if (entity->name == name)
        {
            return entity;
        }
    }

    return nullptr;
}


bool Entity::IsInteractable() const
{
    return type == EntityClassType::Passive ||
        type == EntityClassType::Builder ||
        type == EntityClassType::Vehicle ||
        type == EntityClassType::Cannon ||
        type == EntityClassType::ContainerTransporter ||
        type == EntityClassType::Building ||
        type == EntityClassType::SupplyTransporter ||
        type == EntityClassType::Harvester ||
        type == EntityClassType::Repairer ||
        type == EntityClassType::TransientPassive ||
        type == EntityClassType::Artefact ||
        type == EntityClassType::Sapper ||
        type == EntityClassType::OmnidirectionalEquipment;
}


Node *Entity::CreateModel(Entity *entity, const LObject &obj)
{
    Node *node = nullptr;

    if (entity->IsInteractable())
    {
        InteractableEntity *inter = (InteractableEntity *)entity;

        String<> mesh_name = inter->mesh_name + ".msh";

        Model2150 *model = new Model2150(mesh_name.c_str());

        if (model->IsModel())
        {
            node = model->CreateModel();

            node->SetNodeRotation(Transform3D::MakeRotationZ(obj.rotation_rad));
            Point3D pos = obj._position;
            pos.z = Landscape::GetHeight({ pos.x, pos.y });
            node->SetNodePosition(pos);

            for (int i = 1; i < obj.strings.GetArrayElementCount(); i++)
            {
                Entity *ent = TheParameter->GetEntity(obj.strings[i]);

                if (ent)
                {
                    Node *equipment = Entity::CreateEquipment(ent);

                    if (equipment)
                    {
                        equipment->SetNodePosition({ 0.0f, 0.0, model->_slots.turrets[0].position.z });

                        ((ModelController *)node->GetController())->main_node->AppendNewSubnode(equipment);
                    }
                    else
                    {
                        LOG_ERROR("Can not create equipment %s", ent->name.c_str());
                    }
                }
                else
                {
                    LOG_ERROR("Can not create entity %s", obj.strings[i].c_str());
                }
            }

//            node->AppendNewSubnode(new WorldGizmo(1.0f, 0.01f));

            TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(node);
        }

//        delete model;
    } //-V773
    else
    {
        LOG_ERROR("Can not create entity %s", entity->name.c_str());
    }

    return node;
}


Node *Entity::CreateEquipment(Entity *entity)
{
    Node *node = nullptr;

    if (entity->IsInteractable())
    {
        InteractableEntity *inter = (InteractableEntity *)entity;

        String<> mesh_name = inter->mesh_name + ".msh";

        Model2150 *model = new Model2150(mesh_name.c_str());

        if (model->IsModel())
        {
            node = model->CreateModel();
        }

//        delete model;
    } //-V773

    return node;
}


void EntityGroup::Destroy()
{
    for (Entity *entity : entities)
    {
        delete entity;
    }

    entities.PurgeArray();
}
