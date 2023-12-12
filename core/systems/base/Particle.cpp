#include "Particle.h"
#include "components/Particle.h"
#include "components/Shapes.h"

namespace tZ
{

void SParticle::Update(float deltaSeconds) {
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CEmitter, CTransform>())
    {
        CEmitter& emitter = registry.get<CEmitter>(entity);
        CTransform& transform = registry.get<CTransform>(entity);
        emitter.Time += deltaSeconds;
        if(emitter.Time > emitter.Frequency && emitter.ParticleCount < emitter.MaxParticles)
        {
            u32 particlesToSpawn = (u32)(emitter.Time / emitter.Frequency);
            for(u32 i = 0; i < particlesToSpawn; i++)
            {
                emitter.ParticleCount++;
                entt::entity e = registry.create();
                CParticle pc = {};
                pc.emitterId = entity;
                emitter.spawner(e, transform, registry, pc);
            }
            emitter.Time = 0;
        }
    }

    for(const entt::entity& entity : registry.view<CParticle>())
    {
        CParticle& pc = registry.get<CParticle>(entity);
        pc.Lifetime += deltaSeconds;
        if(pc.Lifetime > pc.MaxLife) {
            CEmitter* ec = registry.try_get<CEmitter>(pc.emitterId);
            if(ec != nullptr) {
                ec->ParticleCount--;
            }
            registry.destroy(entity);
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CMoveCircle>()) {
        CMoveCircle& mc = Game::GetRegistry().get<CMoveCircle>(entity);
        mc.Radius = mc.Radius - (10.0f * deltaSeconds);
        if(mc.Radius <= 0.3) {
            Game::GetRegistry().destroy(entity);
        }
    }
}

}