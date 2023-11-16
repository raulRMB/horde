#include "ShapeDrawingSystem.h"
#include <entt/entt.hpp>

#include "../Components.h"
#include "../../game/Game.h"
#include <raymath.h>

void ShapeDrawingSystem::Draw()
{
    // DrawCubes
    for(const entt::entity& entity : Game::GetRegistry().view<CubeComponent, Transform>())
    {
        const CubeComponent& cubeComponent = Game::GetRegistry().get<CubeComponent>(entity);
        const Transform& transform = Game::GetRegistry().get<Transform>(entity);

        DrawCube(cubeComponent.Position, cubeComponent.Size.x, cubeComponent.Size.y, cubeComponent.Size.z, cubeComponent.Color);
    }

    // DrawCapsules
    for(const entt::entity& entity : Game::GetRegistry().view<CapsuleComponent, Transform>())
    {
        const CapsuleComponent& capsuleComponent = Game::GetRegistry().get<CapsuleComponent>(entity);
        const Transform& transform = Game::GetRegistry().get<Transform>(entity);
        Vector3 offset = {0.0f, capsuleComponent.Height / 2.0f, 0.0f};
        Vector3 top = Vector3Add(capsuleComponent.Position, offset);
        Vector3 bottom = Vector3Subtract(capsuleComponent.Position, offset);
        DrawCapsule(top, bottom,  capsuleComponent.Radius, 10, 10, capsuleComponent.Color);
    }
}
