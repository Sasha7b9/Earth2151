// 2025/03/05 10:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"
#include "Earth2150/Earth2150Types.h"


struct AnimLine
{
    int start;
    int end;

    void Read(FileReader &);
};


struct EntityClassType
{
    enum E
    {
        None = 0x00000000,

        Vehicle = 0x00c00101,
        SupplyTransporter = 0x01c00101,
        Builder = 0x02c00101,
        Harvester = 0x04c00101,
        Sapper = 0x08c00101,

        Equipment = 0x00000002,
        Cannon = 0x00000102,
        Repairer = 0x00000202,
        ContainerTransporter = 0x00000402,
        OmnidirectionalEquipment = 0x00000802,
        UpgradeCopula = 0x00001002,
        TransporterHook = 0x00002002,

        Passive = 0x00000201,
        Mine = 0x00000801,

        MultiExplosion = 0x00010004,

        Building = 0x00010101,
        BuildPassive = 0x00010201,
        Missile = 0x00010401,

        Platoon = 0x00020101,
        TransientPassive = 0x00020201,

        Artefact = 0x01020201,

        Explosion = 0x00020401,
        ExplosionEx = 0x01020401,

        FlyingWaste = 0x00040401,
        StartingPositionMark = 0x00080101,
        Smoke = 0x00080401,

        WallLaser = 0x01100401,
        BuilderLine = 0x02100401,

        BuildingTransporter = 0x01040101,
        ResourceTransporter = 0x02040101,
        UnitTransporter = 0x04040101
    };

    static pchar ToString(E);
};


struct Faction
{
    enum E
    {
        Neutral,
        UCS,
        ED,
        LC
    };

    pchar ToString();

    E value;
};


struct EntityGroupType
{
    enum E
    {
        Unknown,
        Vehicle,
        Cannon,
        Missile,
        Building,
        Special,
        Equipment,
        ShieldGenerator,
        SoundPack,
        SpecialUpdatesLink,
        Parameter
    };

    E value;

    pchar ToString();
};


struct Entity
{
    Entity(const String<> &_name, const Array<int> &_researches, EntityClassType::E _type)
    {
        name = _name;
        type = _type;

        for (int i = 0; i < _researches.GetArrayElementCount(); i++)
        {
            researches.AppendArrayElement(_researches[i]);
        }
    }

    virtual ~Entity()
    {
        researches.PurgeArray();
        filedTypes.PurgeArray();
    }

    String<> name;

    EntityClassType::E type;

    Array<int> researches;
    Array<bool> filedTypes;

    static Entity *Create(FileReader &, EntityGroupType::E);

    static Node *CreateModel(Entity *, const LObject &);

    static Node *CreateEquipment(Entity *);

    virtual void Save(int i, File &file);

    bool IsInteractable() const;
};


struct EntityGroup
{
    Array<Entity *> entities;

    EntityGroupType group_type;

    Faction faction;

    bool Read(FileReader &);

    void Save(int i, File &);

    Entity *GetEntity(pchar);

    void Destroy();
};
