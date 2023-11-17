//
// Created by Raul on 11/17/2023.
//

#include "PhysicsSystem.h"
#include "../../game/Game.h"
#include <entt/entt.hpp>

#include "raymath.h"
#include "../Components.h"
#include "../util/raymathEx.h"


void PhysicsSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<Transform, Physics2DComponent>())
    {
        Transform& transform = registry.get<Transform>(entity);
        Physics2DComponent& physics = registry.get<Physics2DComponent>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
        transform.translation += velocity * deltaSeconds;
    }
}
