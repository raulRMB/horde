#ifndef HORDE_PARTICLE_COMPONENT_H
#define HORDE_PARTICLE_COMPONENT_H

#include "entt/entt.hpp"
#include "components/Transform.h"

namespace tZ
{

struct FGradientColor
{
    f32 Time{};
    FColor Color{};
};

struct CParticle
{
    f32 MaxLife;
    f32 Lifetime;
    entt::entity EmitterId;
    FColor Color;
    std::vector<FGradientColor> ColorsOverLife;
    u8 GradientColorIndex;

    CParticle() : MaxLife(1.0f), Lifetime(0.0f), Color(0xFFFFFFFF),
        EmitterId(entt::null), ColorsOverLife({}), GradientColorIndex(0) {}
};

typedef void (*Spawner)(entt::entity, CTransform&, entt::registry&, CParticle);

struct CEmitter
{
    f32 Frequency;
    f32 Time;
    i32 MaxParticles;
    i32 ParticleCount;
    Spawner spawner;
};

}

#endif //HORDE_PARTICLE_COMPONENT_H
