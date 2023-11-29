//
// Created by Raul on 11/17/2023.
//

#include "Physics.h"
#include "entt/entt.hpp"
#include "raymath.h"
#include "util/raymathEx.h"
#include "util/Util.h"

std::set<entt::entity> toDestroy;

void SPhysics::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<Transform, CPhysics2D, CAttributes>())
    {
        Transform& transform = registry.get<Transform>(entity);
        CAttributes& ac = registry.get<CAttributes>(entity);
        FAttribute* moveSpeed = Util::GetAttribute(ac, "moveSpeed");

        if(moveSpeed->id != "empty") {
            CPhysics2D &physics = registry.get<CPhysics2D>(entity);
            physics.Velocity += physics.Acceleration * deltaSeconds;
            physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
            Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
            velocity *= moveSpeed->get();
            transform.translation += velocity * deltaSeconds;
        }
    }

    auto entities = registry.view<CPhysics2D, Transform>();
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        for (auto jt = std::next(it); jt != entities.end(); ++jt) {
            entt::entity x = *it;
            entt::entity y = *jt;
            CPhysics2D &xP = entities.get<CPhysics2D>(x);
            Transform &xT = entities.get<Transform>(x);
            CPhysics2D &yP = entities.get<CPhysics2D>(y);
            Transform &yT = entities.get<Transform>(y);
            if(Util::Check2DCollision(xP, xT, yP, yT)) {
                auto ac = registry.try_get<CAttributes>(y);
                auto eff = registry.try_get<FEffect>(x);
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

    for(const entt::entity& entity : registry.view<CPhysics2D, Transform>())
    {
        Transform& transform = registry.get<Transform>(entity);
        CPhysics2D& physics = registry.get<CPhysics2D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
        transform.translation += velocity * deltaSeconds;
    }
    for(const entt::entity& entity : registry.view<Transform, CPhysics3D>())
    {
        Transform& transform = registry.get<Transform>(entity);
        CPhysics3D& physics = registry.get<CPhysics3D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = Vector3ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        transform.translation += physics.Velocity * deltaSeconds;
    }
}
