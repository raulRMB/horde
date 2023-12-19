#ifndef HORDE_PARTICLE_COMPONENT_H
#define HORDE_PARTICLE_COMPONENT_H

#include "entt/entt.hpp"
#include "components/Transform.h"
#include "components/TechXComponent.h"

namespace tZ
{

struct FGradientColor
{
    f32 Time{};
    FColor Color{};
};

struct CParticle : CComponent
{
    f32 MaxLife;
    f32 Lifetime;
    entt::entity EmitterId;
    FColor Color;
    std::vector<FGradientColor> ColorsOverLife;
    u8 GradientColorIndex;

    CParticle() : MaxLife(1.0f), Lifetime(0.0f), Color(0xFFFFFFFF),
        EmitterId(entt::null), ColorsOverLife({}), GradientColorIndex(0),
        CComponent("Particle") {}
};

typedef void (*FSpawner)(entt::entity, CTransform&, entt::registry&, CParticle);

struct CEmitter : CComponent
{
    f32 Frequency;
    f32 Time;
    i32 MaxParticles;
    i32 ParticleCount;
    FSpawner Spawner;

    CEmitter(f32 frequency = 1.f, i32 maxParticles = 100, FSpawner spawner = nullptr) : Frequency(frequency), Time(0.0f), MaxParticles(maxParticles), ParticleCount(0), Spawner(spawner),
        CComponent("Emitter") {}
};

}

#endif //HORDE_PARTICLE_COMPONENT_H
