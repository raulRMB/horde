#include "Particle.h"
#include "raylib.h"
#include "components/Particle.h"
#include "components/Shapes.h"

void SParticle::Update(float deltaSeconds) {
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CEmitter, Transform>())
    {
        CEmitter& emitter = registry.get<CEmitter>(entity);
        Transform& transform = registry.get<Transform>(entity);
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

    for(const entt::entity& entity : Game::GetRegistry().view<MoveCircle>()) {
        MoveCircle& mc = Game::GetRegistry().get<MoveCircle>(entity);
        mc.radius = mc.radius - (10.0f * deltaSeconds);
        if(mc.radius <= 0.3) {
            Game::GetRegistry().destroy(entity);
        }
    }
}