#include "Particle.h"
#include "components/Particle.h"

namespace tZ
{

void SParticle::Update(float deltaSeconds) {
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CEmitter, CTransform>())
    {
        CEmitter& emitter = registry.get<CEmitter>(entity);
        CTransform& transform = registry.get<CTransform>(entity);
        emitter.Time += deltaSeconds;
        if(emitter.Time > emitter.Frequency && emitter.ParticleCount < emitter.MaxParticles) {
            emitter.ParticleCount++;
            emitter.Time = 0;
            entt::entity e = registry.create();
            CParticle pc = {};
            pc.emitterId = entity;
            emitter.spawner(e, transform, registry, pc);
        }
    }

    for(const entt::entity& entity : registry.view<CParticle>())
    {
        CParticle& pc = registry.get<CParticle>(entity);
        pc.Lifetime += deltaSeconds;
        if(pc.Lifetime > pc.MaxLife) {
            auto ec = registry.try_get<CEmitter>(pc.emitterId);
            if(ec != nullptr) {
                ec->ParticleCount--;
            }
            registry.destroy(entity);
        }
    }
}

}