// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Interface/Controls/Button.h"


struct ApplicationMode
{
    enum E
    {
        Game,
        Editor
    };

    static void Set(E);

    static bool IsGame()
    {
        return current == Game;
    }

private:

    static E current;
};


struct TypeCampaign
{
    enum E
    {
        ED,
        UCS,
        LC,
        Count
    };

    static void Set(E v)
    {
        current = v;
    }

    static E Current()
    {
        return current;
    }

    static pchar Name();

    // В этом файле хранится кампания
    static String<> FileCampaign();

private:

    static E current;
};



struct GameSetting
{
    GameSetting(pchar _name) : name(_name) { }

    void Set(int);

    int Current();

protected:

    void Init();

private:

    pchar name;
};


struct TypeGame : public GameSetting
{
    enum E
    {
        EftBP,      // Escape from the Blue Planet
        MP,         // The Moon Project
        LS,         // Lost Souls
        Count
    };

    TypeGame() : GameSetting("type_game") { }

    void Set(E);
    bool IsEftBP()
    {
        return Current() == EftBP;
    }
    bool IsMP()
    {
        return Current() == MP;
    }
    bool IsLS()
    {
        return Current() == LS;
    }
    E Current();
};


struct Language : public GameSetting
{
    enum E
    {
        English,
        Polish,
        Russian,
        Count
    };

    Language() : GameSetting("language") { }

    void Set(int);
};


struct SET
{                            // Settings
    struct WINDOW
    {                     // Main window
        static Vector2D SIZE()
        {
            return Vector2D((float)TheEngine->GetVariable("displayWidth")->GetIntegerValue(), (float)TheEngine->GetVariable("displayHeight")->GetIntegerValue());
        };
    };

    struct GUI
    {                        // 
        struct PANEL
        {
            static float SPEED()
            {
                return 0.5F;
            };
        };

        struct MAP
        {
            static float WIDTH()
            {
                return 200.0F;
            };
            static float HEIGHT()
            {
                return SET::GUI::MAP::WIDTH();
            };
            static Vector2D SIZE()
            {
                return Vector2D(WIDTH(), HEIGHT());
            };
            struct VIEW
            {
                static float X()
                {
                    return 0.0F;
                };
                static float Y()
                {
                    return SET::WINDOW::SIZE().y - SET::GUI::MAP::HEIGHT() - SET::GUI::BOTTOM::HEIGHT() + 1.0F;
                };
            };
            struct HIDE
            {
                static float X()
                {
                    return -GUI::MAP::WIDTH();
                };
                static float Y()
                {
                    return SET::GUI::MAP::VIEW::Y();
                };
            };
        };
        struct MAIN
        {                   // Main panel in game GUI
            static float WIDTH()
            {
                return SET::WINDOW::SIZE().x - SET::GUI::MAP::WIDTH() + 1;
            };
            static float HEIGHT()
            {
                return SET::GUI::MAP::HEIGHT();
            };
            static Vector2D SIZE()
            {
                return Vector2D(WIDTH(), HEIGHT());
            };
            struct VIEW
            {
                static float X()
                {
                    return SET::GUI::MAP::WIDTH() - 1.0F;
                };
                static float Y()
                {
                    return SET::GUI::MAP::VIEW::Y();
                };
            };
            struct HIDE
            {
                static float X()
                {
                    return SET::GUI::MAIN::VIEW::X();
                };
                static float Y()
                {
                    return SET::WINDOW::SIZE().y;
                };
            };
        };
        struct TAB_WIDGET
        {
            struct BORDER
            {
                static int LEFT()
                {
                    return 5;
                };
                static int RIGHT()
                {
                    return LEFT();
                };
                static int BOTTOM()
                {
                    return RIGHT();
                };
                static int TOP()
                {
                    return 25;
                };
            };
        };
        struct BOTTOM
        {                 // Lower panel of game TheGUI
            static float WIDTH()
            {
                return SET::WINDOW::SIZE().x;
            };
            static float HEIGHT()
            {
                return 20.0F;
            };
            static Vector2D SIZE()
            {
                return Vector2D(WIDTH(), HEIGHT());
            };
            static float X()
            {
                return 0.0f;
            };
            static float Y()
            {
                return SET::WINDOW::SIZE().y - SET::GUI::BOTTOM::HEIGHT();
            };
        };
        struct BUTTON
        {                 // Buttom of GUI
            static float WIDTH(TButton::Type type)
            {
                switch (type)
                {
                case TButton::Type::Normal:    return 110.0f;  break;
                case TButton::Type::NameTab:   return 75.0f;  break;
                }
                return 75;
            };
            static float HEIGHT(TButton::Type type)
            {
                switch (type)
                {
                case TButton::Type::Normal:    return 16.0f; break;
                case TButton::Type::NameTab:   return 21.0f; break;
                }
                return 20;
            };
            static Vector2D SIZE(TButton::Type type)
            {
                return Vector2D(WIDTH(type), HEIGHT(type));
            }
            struct COLOR
            {
                static ColorRGBA BACKGROUND(TButton::Type type)
                {
                    switch (type)
                    {
                    case TButton::Type::Normal:    return ColorRGBA(0.1f, 0.1f, 0.1f);  break;
                    case TButton::Type::NameTab:   return ColorRGBA(0.01f, 0.01f, 0.01f);  break;
                    };
                    return Color::black;
                }
            };
        };

        struct MENU_GAME
        {
            static Vector2D SIZE()
            {
                return Vector2D(SET::GUI::BUTTON::WIDTH(TButton::Type::Normal), (SET::GUI::BUTTON::HEIGHT(TButton::Type::Normal) - 1) * 3);
            }
        };

        struct MENU_MAIN
        {
            static Vector2D SIZE()
            {
                return Vector2D(200, 300);
            }
        };

        struct CHECKBOX
        {
            static float WIDTH()
            {
                return 75.0F;
            };
            static float HEIGHT()
            {
                return 16.0F;
            };
            static Vector2D SIZE()
            {
                return Vector2D(WIDTH(), HEIGHT());
            };
            struct ZONE
            {
                static float X()
                {
                    return 2.0F;
                };
                static float Y()
                {
                    return 2.0F;
                };
                static float WIDTH()
                {
                    return SET::GUI::CHECKBOX::HEIGHT() - 2.0F - 2.0F * X();
                };
                static float HEIGHT()
                {
                    return WIDTH();
                };
                struct COLOR
                {
                    static ColorRGBA BORDER()
                    {
                        return Color::white;
                    };
                    static ColorRGBA FILL()
                    {
                        return Color::blue;
                    };
                };
            };
        };

        struct CONSOLE
        {
            static Vector2D SIZE()
            {
                return Vector2D(400.0F, 200.0F);
            }
        };

        struct COLOR
        {
            static ColorRGBA BORDER()
            {
                return ColorRGBA(20.0F, 20.0F, 20.0F);
            };
            static ColorRGBA BACKGROUND()
            {
                return Color::black;
            };
            static ColorRGBA FILL()
            {
                return ColorRGBA(50.0F, 50.0F, 50.0F);
            };
            static ColorRGBA CHECK()
            {
                return BACKGROUND();
            };
        };

    };
};
