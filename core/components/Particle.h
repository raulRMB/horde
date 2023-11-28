#ifndef HORDE_PARTICLE_COMPONENT_H
#define HORDE_PARTICLE_COMPONENT_H

#include <entt/entt.hpp>

struct CParticle
{
    float MaxLife;
    float Lifetime;
    entt::entity emitterId;
};

typedef void (*Spawner)(entt::entity, struct Transform&, entt::registry&, CParticle);

struct CEmitter
{
    float Frequency;
    float Time;
    int MaxParticles;
    int ParticleCount;
    Spawner spawner;
};

#endif //HORDE_PARTICLE_COMPONENT_H
