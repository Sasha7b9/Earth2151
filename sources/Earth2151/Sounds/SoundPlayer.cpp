// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "SoundPlayer.h"


SoundPlayer *TheSoundPlayer = nullptr;


SoundPlayer::SoundPlayer() : Global<SoundPlayer>(TheSoundPlayer)
{
    sounds[kMovePanel] = "sounds/shoot";
}


SoundPlayer::~SoundPlayer()
{
}


void SoundPlayer::Play(uint number)
{
    (void)number;
    return;
//    Sound *sound = new Sound();
//    sound->LoadSound(sounds[number]);
//    sound->Play();
} //-V773
