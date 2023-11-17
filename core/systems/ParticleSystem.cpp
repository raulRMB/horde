#include "ParticleSystem.h"

#include <raylib.h>
#include <raymath.h>
#include "../util/raymathEx.h"
#include "../Components.h"
#include "../../game/SmartEntity.h"

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
            ParticleComponent pc = {};
            pc.emitterId = entity;
            pc.MaxLife = 3;
            pc.Lifetime = 0;

            CapsuleComponent cc = {};

            cc.Color = Color{
                    (unsigned char)GetRandomValue(1, 255),
                    (unsigned char)GetRandomValue(1, 255),
                    (unsigned char)GetRandomValue(1, 255),
                    255};
            cc.Height = 0.5;
            cc.Position = Vector3{0,0,0};
            cc.Radius = 0.5;
            cc.Slices = 2;

            entt::entity e = registry.create();
            registry.emplace<ParticleComponent>(e, pc);
            registry.emplace<CapsuleComponent>(e, cc);

            Transform t = {};
            t.translation = Vector3{
                (float)GetRandomValue(-1, 1),
                (float)GetRandomValue(-1, 1),  
                (float)GetRandomValue(-1, 1)
            };
            t.translation = transform.translation + t.translation;
            registry.emplace<Transform>(e, t);
        }
    }

    for(const entt::entity& entity : registry.view<ParticleComponent>())
    {
        ParticleComponent& pc = registry.get<ParticleComponent>(entity);
        pc.Lifetime += deltaSeconds;
        if(pc.Lifetime > pc.MaxLife) {
            EmitterComponent& ec = registry.get<EmitterComponent>(pc.emitterId);
            ec.ParticleCount--;
            registry.destroy(entity);
        }
    }
}