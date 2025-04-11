// 2025/03/05 09:08:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"


struct FileFlags
{
    enum E
    {
        _None = 0,
        Compressed = 1,
        Archive = 2,
        Text = 4,
        Names = 8,
        Resource = 16,
        GUID = 32
    };

    E value = _None;

    bool IsNamed() const
    {
        return (value & Names) != 0;
    }

    bool IsResource() const
    {
        return (value & Resource) != 0;
    }

    bool IsGUID() const
    {
        return (value & GUID) != 0;
    }
};


struct ResType
{
    enum E
    {
        _None,
        Interface = 0x49,
        MainInterface = 0xff,
        Map = 0x4c,
        MapAssets = 0x4d,
        Parameters = 0x99,
        Terrain = 0x54,
        Asset = 0x01,

        TankScript = 0x02,
        SapperScript = 0x03,
        HarvesterScript = 0x04,
        CarrierScript = 0x05,
        RepairerScript = 0x06,
        SupplierScript = 0x07,
        BuilderScript = 0x08,
        AircraftScript = 0x09,
        CivilScript = 0x0a,
        PlatoonScript = 0x0b,
        CampaignScript = 0x0c,
        PlayerScript = 0x0d,
        MissionScript = 0x0e,
        TransporterScript = 0x10
    };

    E value = _None;
};


struct InfoHeader
{
    String<> file_path;

    FileFlags flags;

    uint8 guid[16];

    ResType resourceType;

    String<> translationId;

    // first_byte - номер первого байта ("PAR")
    bool Fill(FileReader &);
};
