#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "util/Util.h"
#include "components/Attribute.h"
#include <entt/entt.hpp>
#include "components/Particle.h"
#include "components/Physics.h"
#include "components/Shapes.h"

namespace tZ
{

Spawner spawnParticle = [](entt::entity e, CTransform& transform, entt::registry& registry, CParticle pc) {

    pc.MaxLife = 0.2;

    CCapsule cc = {};
    cc.Color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = v3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    CPhysics3D phc = {};
    phc.Velocity = Util::RandVec3(-2, 2);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-1, 1);

    CTransform t = {};
    t.Position = Util::RandVec3(-1, 1);
    t.Position += transform.Position;

    registry.emplace<CParticle>(e, pc);
    registry.emplace<CCapsule>(e, cc);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<CTransform>(e, t);
};

void Projectile(entt::entity source, v3 cursorLocation, v3 playerLocation)
{
    entt::registry& registry = Game::GetRegistry();

    OnApply effectCallback = [](CAttributes &target, CAttributes &source) {
        FAttribute &health = *Util::GetAttribute(target, "health");
        float newHealth = health.base - 150;
        health.base = std::clamp(newHealth, health.min, health.max);
    };
    FEffect effect = FEffect{};
    effect.type = EEffectType::Instant;
    effect.source = source;
    effect.callback = effectCallback;

    auto e = registry.create();
    registry.emplace<CEmitter>(e,
                                        CEmitter{.Frequency=0.01, .MaxParticles=100, .spawner=spawnParticle});
    CPhysics3D phc = {};
    phc.Velocity = glm::normalize(cursorLocation - playerLocation) * 50.f;
    phc.MaxSpeed = 100;
    phc.Acceleration = v3{1, 1, 1};

    CPhysics2D p2d = {};
    p2d.CollisionType = ECollision2DType::Circle;
    p2d.CollisionRadius = 1.f;
    registry.emplace<CPhysics2D>(e, p2d);
    registry.emplace<FEffect>(e, effect);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<CTransform>(e, playerLocation + glm::normalize(cursorLocation - playerLocation) * 3.f);
}


}
#endif //PROJECTILE_H
