//
// Created by Raul on 11/17/2023.
//

#include "PhysicsSystem.h"
#include "entt/entt.hpp"
#include "raymath.h"
#include "components/Components.h"
#include "util/raymathEx.h"
#include "util/Util.h"


void PhysicsSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<Transform, Physics2DComponent, AttributesComponent>())
    {
        Transform& transform = registry.get<Transform>(entity);
        AttributesComponent ac = registry.get<AttributesComponent>(entity);
        Attribute& moveSpeed = Util::GetAttribute(ac, "moveSpeed");

        if(moveSpeed.id != "empty") {
            Physics2DComponent &physics = registry.get<Physics2DComponent>(entity);
            physics.Velocity += physics.Acceleration * deltaSeconds;
            physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
            Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
            velocity *= moveSpeed.get();
            transform.translation += velocity * deltaSeconds;
        }
    }
    for(const entt::entity& entity : registry.view<Transform, Physics2DComponent>())
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
