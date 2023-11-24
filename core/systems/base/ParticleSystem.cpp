#include "ParticleSystem.h"
#include "raylib.h"
#include "util/raymathEx.h"
#include "components/Components.h"

void ParticleSystem::Update(float deltaSeconds) {
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<EmitterComponent, Transform>())
    {
        EmitterComponent& emitter = registry.get<EmitterComponent>(entity);
        Transform& transform = registry.get<Transform>(entity);
        emitter.Time += deltaSeconds;
        if(emitter.Time > emitter.Frequency && emitter.ParticleCount < emitter.MaxParticles) {
            emitter.ParticleCount++;
            emitter.Time = 0;
            entt::entity e = registry.create();
            ParticleComponent pc = {};
            pc.emitterId = entity;
            emitter.spawner(e, transform, registry, pc);
        }
    }

    for(const entt::entity& entity : registry.view<ParticleComponent>())
    {
        ParticleComponent& pc = registry.get<ParticleComponent>(entity);
        pc.Lifetime += deltaSeconds;
        if(pc.Lifetime > pc.MaxLife) {
            auto ec = registry.try_get<EmitterComponent>(pc.emitterId);
            if(ec != nullptr) {
                ec->ParticleCount--;
            }
            registry.destroy(entity);
        }
    }
}