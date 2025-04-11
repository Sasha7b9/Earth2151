// 2025/02/01 23:30:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Graphics/Particles.h"


class SmokePowerPlaint : public PointParticleSystem
{
private:

    enum
    {
        kMaxParticleCount = 10000
    };

    ColorRGB        smokeColor;

    ParticlePool<>  particlePool;
    Particle        particleArray[kMaxParticleCount];

    SmokePowerPlaint() :
        PointParticleSystem(kParticleSmokePowerPlaint, &particlePool, "particle/Trail"),
        particlePool(kMaxParticleCount, particleArray) { }

public:

    SmokePowerPlaint(const ColorRGB &color) :
        PointParticleSystem(kParticleSmokePowerPlaint, &particlePool, "particle/Trail"),
        smokeColor(color),
        particlePool(kMaxParticleCount, particleArray)
    {
    }

    ~SmokePowerPlaint() { }

    void FinalizeParticleSystem()
    {
        SetParticleSystemFlags(GetParticleSystemFlags() | kParticleSystemSelfDestruct);
    }

    void Pack(Packer &data, uint32 packFlags) const override
    {
        PointParticleSystem::Pack(data, packFlags);

        data << ChunkHeader('COLR', sizeof(ColorRGB));
        data << smokeColor;

        data << Packer::terminator;
    }

    void Unpack(Unpacker &data, uint32 unpackFlags) override
    {
        PointParticleSystem::Unpack(data, unpackFlags);
        UnpackChunkList<SmokePowerPlaint>(data, unpackFlags);
    }

    void UnpackChunk(const ChunkHeader *chunkHeader, Unpacker &data, uint32 unpackFlags)
    {
        switch (chunkHeader->chunkType)
        {
        case 'COLR':

            data >> smokeColor;
            break;
        }
    }

    void PreprocessNode() override
    {
        SetParticleSystemFlags(kParticleSystemSoftDepth | kParticleSystemDepthRamp);
        PointParticleSystem::PreprocessNode();

        SetAmbientBlendState(kBlendInterpolate);

        SetEffectListIndex(kEffectListPartialLight);
        SetLightBlendState(BlendState(kBlendSourceAlpha, kBlendOne));

        SetSoftDepthScale(2.0F);
        SetDepthRampRange(Range<float>(0.125F, 1.0F));
    }

    void AnimateParticles() override
    {
        int32 dt = TheTimeMgr->GetDeltaTime();
        float fdt = TheTimeMgr->GetFloatDeltaTime();

        Particle *particle = GetFirstParticle();
        while (particle)
        {
            float dist = fdt * 0.0008f;

            float d = 0.5f;

            float k = 20.0f;

            particle->position.x += Random::RandomFloat(-dist / d / k, dist / d);
            particle->position.y += Random::RandomFloat(-dist / d / k, dist / d);
            particle->position.z += Random::RandomFloat(0.0f, dist * 2.0f);

            Particle *next = particle->nextParticle;

            int32 life = (particle->lifeTime -= dt);

            if (particle->position.z > 3.0f)
            {
                FreeParticle(particle);
            }
            else if (life > 0)
            {
                particle->radius += fdt * 0.00008F;
            }
            else
            {
                FreeParticle(particle);
            }

            particle = next;
        }
    }

    void CreateSmoke(const Point3D &position, int32 life, float radius)
    {
        Particle *particle = particlePool.NewParticle();
        if (particle)
        {
            particle->emitTime = 0;
            particle->lifeTime = life;
            particle->radius = radius;

            float c = Random::RandomFloat(0.5F);
            c = c * c + 0.1F;
            particle->color.Set(smokeColor * c, 0.5F);

            particle->orientation = Random::RandomInteger(256) << 24;
            particle->position = position;

            AddFarthestParticle(particle);
        }
    }
};


class SmokeRocket : public PointParticleSystem
{
private:

    enum
    {
        kMaxParticleCount = 1000000
    };

    ColorRGB        smokeColor;

    ParticlePool<>  particlePool;
    Particle        particleArray[kMaxParticleCount];

    SmokeRocket() :
        PointParticleSystem(kParticleSmokeRocket, &particlePool, "particle/Trail"),
        particlePool(kMaxParticleCount, particleArray)
    {
    }

public:

    SmokeRocket(const ColorRGB &color) :
        PointParticleSystem(kParticleSmokeRocket, &particlePool, "particle/Trail"),
        smokeColor(color),
        particlePool(kMaxParticleCount, particleArray)
    {
    }

    ~SmokeRocket()
    {
    }

    void FinalizeParticleSystem()
    {
        SetParticleSystemFlags(GetParticleSystemFlags() | kParticleSystemSelfDestruct);
    }

    void Pack(Packer &data, uint32 packFlags) const override
    {
        PointParticleSystem::Pack(data, packFlags);

        data << ChunkHeader('COLR', sizeof(ColorRGB));
        data << smokeColor;

        data << Packer::terminator;
    }

    void Unpack(Unpacker &data, uint32 unpackFlags) override
    {
        PointParticleSystem::Unpack(data, unpackFlags);
        UnpackChunkList<SmokePowerPlaint>(data, unpackFlags);
    }

    void UnpackChunk(const ChunkHeader *chunkHeader, Unpacker &data, uint32 unpackFlags)
    {
        switch (chunkHeader->chunkType)
        {
        case 'COLR':

            data >> smokeColor;
            break;
        }
    }

    void PreprocessNode() override
    {
        SetParticleSystemFlags(kParticleSystemSoftDepth | kParticleSystemDepthRamp);
        PointParticleSystem::PreprocessNode();

        SetAmbientBlendState(kBlendInterpolate);

        SetEffectListIndex(kEffectListPartialLight);
        SetLightBlendState(BlendState(kBlendSourceAlpha, kBlendOne));

        SetSoftDepthScale(2.0F);
        SetDepthRampRange(Range<float>(0.125F, 1.0F));
    }

    void AnimateParticles() override
    {
        float fdt = TheTimeMgr->GetFloatDeltaTime();

        Particle *particle = GetFirstParticle();

        while (particle)
        {
            Particle *next = particle->nextParticle;

            particle->lifeTime -= fdt;

            particle->radius += fdt * 0.00008F;

            float speed_alpha = 0.0001f;

            particle->color.alpha -= speed_alpha * fdt;

            if (particle->color.alpha < 0.0001f)
            {
                FreeParticle(particle);
            }

            particle = next;
        }
    }

    void CreateSmoke(const Point3D &position, int32 life, float radius)
    {
        Particle *particle = particlePool.NewParticle();
        if (particle)
        {
            particle->emitTime = 0;
            particle->lifeTime = life;
            particle->radius = radius;

            float c = Random::RandomFloat(0.5F);
            c = c * c + 0.1F;
            particle->color.Set(smokeColor * c, 0.5F);

            particle->orientation = Random::RandomInteger(256) << 24;
            particle->position = position;

            AddFarthestParticle(particle);
        }
    }
};


namespace Particles
{
    static SmokePowerPlaint *smokePowerPlaint = nullptr;
    static SmokeRocket *smokeRocket = nullptr;
}


void Particles::Init()
{
    smokePowerPlaint = new SmokePowerPlaint(Color::white);

    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(smokePowerPlaint);

    smokeRocket = new SmokeRocket(Color::white);

    TheWorldMgr->GetWorld()->GetRootNode()->AppendNewSubnode(smokeRocket);
}


void Particles::DeInit()
{
    if (smokePowerPlaint)
    {
        smokePowerPlaint->FinalizeParticleSystem();
    }

    if (smokeRocket)
    {
        smokeRocket->FinalizeParticleSystem();
    }
}


void Particles::Create(ParticleSystemType type, const Point3D &coord)
{
    if (type == kParticleSmokePowerPlaint)
    {
        static Mutex mutex;
        mutex.Acquire();
        smokePowerPlaint->CreateSmoke(coord, 7500, 0.1f);
        mutex.Release();
    }
    else if (type == kParticleSmokeRocket)
    {
        static Mutex mutex;
        mutex.Acquire();
        smokeRocket->CreateSmoke(coord, 10000, 0.5f);
        mutex.Release();
    }
}
