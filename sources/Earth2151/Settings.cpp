// 2025/03/22 17:19:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Settings.h"
#include "Utils/Locale.h"
#include "Earth2151.h"


ApplicationMode::E ApplicationMode::current = ApplicationMode::Game;
TypeCampaign::E TypeCampaign::current = TypeCampaign::Count;


pchar TypeCampaign::Name()
{
    static const pchar names[Count] =
    {
        "ED",
        "UCS",
        "LC"
    };

    return names[current];
}


void ApplicationMode::Set(ApplicationMode::E mode)
{
    current = mode;
}


void NameGame::Set(NameGame::E type)
{
    Init();

    TheEngine->GetVariable("type_game")->SetIntegerValue(type);
}


NameGame::E NameGame::Current()
{
    Init();

    return (E)TheEngine->GetVariable("type_game")->GetIntegerValue();
}


void GameSetting::Set(int set)
{
    Init();

    TheEngine->GetVariable(name)->SetIntegerValue(set);
}


void Language::Set(int set)
{
    GameSetting::Set(set);

    Local::Init();
}


int GameSetting::Current()
{
    Init();

    return TheEngine->GetVariable(name)->GetIntegerValue();
}


void GameSetting::Init()
{
    TheEngine->InitVariable(name, "0");
}


String<> TypeCampaign::FileCampaign()
{
    static const pchar names[NameGame::Count][Count] =
    {
        {
            "Scripts/campaigns/Ed/CampaignED.ec",               // Escape from the Blue Planet
            "Scripts/campaigns/Ucs/CampaignUCS.ec",
            "Scripts/campaigns/Lc/CampaignLC.ec"
        },
        {
            "Scripts/Campaigns/ED/CampaignEDMP01.ecoMP",        // The Moon Project
            "Scripts/Campaigns/UCS/CampaignUCSMP01.ecoMP",
            "Scripts/Campaigns/LC/CampaignLCMP01.ecoMP"
        },
        {
            "Scripts/Campaigns/ED/campaignED_LS.ecoMP",         // Lost Souls
            "Scripts/Campaigns/UCS/campaignUCS_LS.ecoMP",
            "Scripts/Campaigns/LC/campaignlc_ds.ecoMP"
        }
    };

    return RESOURCE_PATH(names[NameGame().Current()][current]);
}


String<> TypeCampaign::FileTutorial()
{
    static const pchar names[NameGame::Count][Count] =
    {
        {
            "Scripts/campaigns/Ed/Missions/TutorialEDmis.ec",
            "Scripts/campaigns/Ucs/Missions/TutorialUCSmis.ec",
            "Scripts/campaigns/Lc/Missions/TutorialLCmis.ec"
        },
        {
            "Scripts/Campaigns/ED/Missions/TutorialEDmis.ecoMP",
            "Scripts/Campaigns/UCS/Missions/TutorialUCSmis.ecoMP",
            "Scripts/Campaigns/LC/Missions/TutorialLCmis.ecoMP"
        },
        {
            "Scripts/Campaigns/ED/Missions/TutorialEDmis.ecoMP",
            "Scripts/Campaigns/UCS/Missions/TutorialUCSmis.ecoMP",
            "Scripts/Campaigns/LC/Missions/TutorialLCmis.ecoMP"
        }
    };

    return RESOURCE_PATH(names[NameGame().Current()][current]);
}
