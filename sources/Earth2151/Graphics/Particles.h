// 2025/02/01 23:30:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


enum : ParticleSystemType
{
    kParticleSmokePowerPlaint = 'pspp',
    kParticleSmokeRocket = 'psrc'
};


namespace Particles
{
    void Init();

    void DeInit();

    void Create(ParticleSystemType, const Point3D &);
}
