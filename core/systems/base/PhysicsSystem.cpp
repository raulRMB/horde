//
// Created by Raul on 11/17/2023.
//

#include "PhysicsSystem.h"
#include "entt/entt.hpp"
#include "raymath.h"
#include "components/Components.h"
#include "util/raymathEx.h"
#include "util/Util.h"

std::set<entt::entity> toDestroy;

void PhysicsSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<Transform, Physics2DComponent, AttributesComponent>())
    {
        if (registry.valid(entity)) {
            Transform &transform = registry.get<Transform>(entity);
            AttributesComponent &ac = registry.get<AttributesComponent>(entity);
            Attribute *moveSpeed = Util::GetAttribute(ac, "moveSpeed");

            if (moveSpeed->id != "empty") {
                Physics2DComponent &physics = registry.get<Physics2DComponent>(entity);
                physics.Velocity += physics.Acceleration * deltaSeconds;
                physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
                Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
                velocity *= moveSpeed->get();
                transform.translation += velocity * deltaSeconds;
            }
        }
    }

    auto entities = registry.view<Physics2DComponent, Transform>();
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        for (auto jt = std::next(it); jt != entities.end(); ++jt) {
            entt::entity x = *it;
            entt::entity y = *jt;
            Physics2DComponent &xP = entities.get<Physics2DComponent>(x);
            Transform &xT = entities.get<Transform>(x);
            Physics2DComponent &yP = entities.get<Physics2DComponent>(y);
            Transform &yT = entities.get<Transform>(y);
            if(Util::Check2DCollision(xP, xT, yP, yT)) {
                auto ac = registry.try_get<AttributesComponent>(y);
                auto eff = registry.try_get<Effect>(x);
                if(ac != nullptr && eff != nullptr) {
                    eff->target = y;
                    Game::GetDispatcher().trigger(*eff);
                    toDestroy.insert(x);
                }
            }
        }
    }

    for(entt::entity x : toDestroy) {
        auto found = registry.try_get<Transform>(x);
        if(found != nullptr) {
            registry.destroy(x);
        }
    }
    toDestroy.empty();

    for(const entt::entity& entity : registry.view<Physics2DComponent, Transform>())
    {
        Transform& transform = registry.get<Transform>(entity);
        Physics2DComponent& physics = registry.get<Physics2DComponent>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
        transform.translation += velocity * deltaSeconds;
    }
    for(const entt::entity& entity : registry.view<Transform, Physics3DComponent>())
    {
        Transform& transform = registry.get<Transform>(entity);
        Physics3DComponent& physics = registry.get<Physics3DComponent>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = Vector3ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        transform.translation += physics.Velocity * deltaSeconds;
    }
}
