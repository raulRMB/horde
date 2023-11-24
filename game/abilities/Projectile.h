#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../core/components/Components.h"
#include "../core/util/Util.h"
#include <entt/entt.hpp>

Spawner spawnParticle = [](entt::entity e, Transform& transform, entt::registry& registry, ParticleComponent pc) {

    pc.MaxLife = 0.2;

    CapsuleComponent cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    Physics3DComponent phc = {};
    phc.Velocity = Util::RandVec3(-2, 2);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-1, 1);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<ParticleComponent>(e, pc);
    registry.emplace<CapsuleComponent>(e, cc);
    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, t);
};

void Projectile(entt::entity source, Vector3 cursorLocation, Vector3 playerLocation) {

    entt::registry& registry = Game::GetRegistry();

    OnApply effectCallback = [](AttributesComponent &target, AttributesComponent &source) {
        Attribute &health = Util::GetAttribute(target, "health");
        float newHealth = health.base - 150;
        health.base = std::clamp(newHealth, health.min, health.max);
    };
    Effect effect = Effect{};
    effect.type = INSTANT;
    effect.source = source;
    effect.callback = effectCallback;

    auto e = registry.create();
    registry.emplace<EmitterComponent>(e,
                                        EmitterComponent{.Frequency=0.01, .MaxParticles=100, .spawner=spawnParticle});
    Physics3DComponent phc = {};
    phc.Velocity = Vector3Normalize(cursorLocation - playerLocation) * 50;
    phc.MaxSpeed = 100;
    phc.Acceleration = Vector3{1, 1, 1};

    Physics2DComponent p2d = {};
    p2d.CollisionType = Circle;
    p2d.CollisionRadius = 1.f;
    registry.emplace<Physics2DComponent>(e, p2d);

    registry.emplace<Effect>(e, effect);

    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, playerLocation + Vector3Normalize(cursorLocation - playerLocation) * 3);
}
#endif //PROJECTILE_H
