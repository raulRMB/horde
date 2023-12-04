#ifndef HORDE_PARTICLE_COMPONENT_H
#define HORDE_PARTICLE_COMPONENT_H

#include <entt/entt.hpp>
#include "components/Transform.h"

namespace tZ
{

struct CParticle
{
    float MaxLife;
    float Lifetime;
    entt::entity emitterId;
};

typedef void (*Spawner)(entt::entity, CTransform&, entt::registry&, CParticle);

struct CEmitter
{
    float Frequency;
    float Time;
    int MaxParticles;
    int ParticleCount;
    Spawner spawner;
};

}

#endif //HORDE_PARTICLE_COMPONENT_H
