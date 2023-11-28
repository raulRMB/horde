#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "util/Util.h"
#include <entt/entt.hpp>

Spawner spawnParticle = [](entt::entity e, Transform& transform, entt::registry& registry, CParticle pc) {

    pc.MaxLife = 0.2;

    CCapsule cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    CPhysics3D phc = {};
    phc.Velocity = Util::RandVec3(-2, 2);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-1, 1);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<CParticle>(e, pc);
    registry.emplace<CCapsule>(e, cc);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<Transform>(e, t);
};

void Projectile(entt::entity source, Vector3 cursorLocation, Vector3 playerLocation) {

    entt::registry& registry = Game::GetRegistry();


    Game::GetClient()->Send();
    OnApply effectCallback = [](CAttributes &target, CAttributes &source) {
        FAttribute &health = *Util::GetAttribute(target, "health");
        float newHealth = health.base - 150;
        health.base = std::clamp(newHealth, health.min, health.max);
    };
    FEffect effect = FEffect{};
    effect.type = INSTANT;
    effect.source = source;
    effect.callback = effectCallback;

    auto e = registry.create();
    registry.emplace<CEmitter>(e,
                                        CEmitter{.Frequency=0.01, .MaxParticles=100, .spawner=spawnParticle});
    CPhysics3D phc = {};
    phc.Velocity = Vector3Normalize(cursorLocation - playerLocation) * 50;
    phc.MaxSpeed = 100;
    phc.Acceleration = Vector3{1, 1, 1};

    CPhysics2D p2d = {};
    p2d.CollisionType = ECollision2DType::Circle;
    p2d.CollisionRadius = 1.f;
    registry.emplace<CPhysics2D>(e, p2d);
    registry.emplace<FEffect>(e, effect);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<Transform>(e, playerLocation + Vector3Normalize(cursorLocation - playerLocation) * 3);
}
#endif //PROJECTILE_H
