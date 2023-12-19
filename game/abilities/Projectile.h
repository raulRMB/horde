#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "util/Util.h"
#include "components/CAttribute.h"
#include <entt/entt.hpp>
#include "components/CParticle.h"
#include "components/CPhysics.h"
#include "components/CShapes.h"
#include "components/CLifetime.h"

namespace tX
{

inline FSpawner spawnParticle = [](entt::entity e, CTransform& transform, entt::registry& registry, CParticle pc) {

    pc.MaxLife = 1.0f;

    CPhysics3D phc = {};
    phc.Velocity = Util::RandVec3(-5, 5);
    phc.MaxSpeed = 100;

    CTransform t = {};
    t.Position = Util::RandVec3(-1, 1) * v3(1., 0., 1.) * 0.6f;
    t.Position += transform.Position;

    pc.ColorsOverLife = {
        {0.0f, FColor{0xFF0000FF}},
        {0.2f, FColor{0xFFFF00C0}},
        {1.0f, FColor{0x00FF0000}},
    };

    registry.emplace<CParticle>(e, pc);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<CTransform>(e, t);
};

inline void Projectile(entt::entity source, v3 cursorLocation, v3 playerLocation, float speed, float lifetime)
{
    entt::registry& registry = Game::GetRegistry();

    OnApply effectCallback = [](CAttributeSet &target, CAttributeSet &source) {
        FAttribute &health = *Util::GetAttribute(target, "health");
        float newHealth = health.base - 150;
        health.base = std::clamp(newHealth, health.min, health.max);
    };
    FEffect effect = FEffect{};
    effect.type = EEffectType::Instant;
    effect.source = source;
    effect.callback = effectCallback;

    entt::entity e = registry.create();
    registry.emplace<CEmitter>(e, CEmitter(0.0001f, 10000, spawnParticle));

    auto lt = CLifetime{};
    lt.MaxLifetime = lifetime;
    registry.emplace<CLifetime>(e, lt);

    CPhysics3D phc = {};
    phc.Velocity = glm::normalize(cursorLocation - playerLocation) * speed;
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

inline void SpawnProjectile(entt::entity source, const v2 pos, const v2 dir, const float speed, const float lifetime)
{
    entt::registry& registry = Game::GetRegistry();

    auto e = registry.create();
    registry.emplace<CEmitter>(e, CEmitter(0.001, 10000, spawnParticle));
    CPhysics3D phc = {};
    phc.Velocity = glm::normalize(v3{dir.x, 0, dir.y}) * speed;
    phc.MaxSpeed = 100;

    OnApply effectCallback = [](CAttributeSet &target, CAttributeSet &source) {
        FAttribute &health = *Util::GetAttribute(target, "health");
        float newHealth = health.base - 150;
        health.base = std::clamp(newHealth, health.min, health.max);
    };
    FEffect effect = FEffect{};
    effect.type = EEffectType::Instant;
    effect.source = source;
    effect.callback = effectCallback;
    registry.emplace<FEffect>(e, effect);

    auto lt = CLifetime{};
    lt.MaxLifetime = lifetime;
    registry.emplace<CLifetime>(e, lt);

    CPhysics2D p2d = {};
    p2d.CollisionType = ECollision2DType::Circle;
    p2d.CollisionRadius = 1.f;
    registry.emplace<CPhysics2D>(e, p2d);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<CTransform>(e, v3(pos.x, 0, pos.y) + glm::normalize(v3{dir.x, 0, dir.y}) * 3.f);
}


}
#endif //PROJECTILE_H
