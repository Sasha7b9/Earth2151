// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


enum
{
    kMovePanel,
    kNumSounds
};

class SoundPlayer : public Global<SoundPlayer>
{
public:

    virtual ~SoundPlayer();

    void Play(uint number);

private:

    SoundPlayer();
    pchar sounds[kNumSounds];
};


extern SoundPlayer *TheSoundPlayer;
