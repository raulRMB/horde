#include "ShapeDrawingSystem.h"
#include <entt/entt.hpp>

#include "../Components.h"
#include "../../game/Game.h"
#include <raymath.h>
#include "../primitives/Triangles.h"

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
    for(const entt::entity& entity : Game::GetRegistry().view<CapsuleComponent>())
    {
        const CapsuleComponent& capsuleComponent = Game::GetRegistry().get<CapsuleComponent>(entity);
        Vector3 offset = {0.0f, capsuleComponent.Height / 2.0f, 0.0f};
        Vector3 top = Vector3Add(capsuleComponent.Position, offset);
        Vector3 bottom = Vector3Subtract(capsuleComponent.Position, offset);
        DrawCapsule(top, bottom,  capsuleComponent.Radius, 5, 3, capsuleComponent.Color);
    }

    // Rays
    for(const entt::entity& entity : Game::GetRegistry().view<RayComponent>())
    {
        const RayComponent& rayComponent = Game::GetRegistry().get<RayComponent>(entity);
        DrawRay(rayComponent.Ray, rayComponent.Color);
    }

    // Draw Triangles
    for(const entt::entity& entity : Game::GetRegistry().view<Triangle2D>())
    {
        const Triangle2D& triangleComponent = Game::GetRegistry().get<Triangle2D>(entity);
        Triangle3D triangle3D{};
        triangle3D.A = {triangleComponent.Vertices.A.x, 0.0f, triangleComponent.Vertices.A.y};
        triangle3D.B = {triangleComponent.Vertices.B.x, 0.0f, triangleComponent.Vertices.B.y};
        triangle3D.C = {triangleComponent.Vertices.C.x, 0.0f, triangleComponent.Vertices.C.y};
        DrawTriangle3D(triangle3D.C, triangle3D.B, triangle3D.A, BLUE);
    }

    // Draw Linestrip
    for(const entt::entity& entity : Game::GetRegistry().view<LineStripComponent>())
    {
        const LineStripComponent& lineStripComponent = Game::GetRegistry().get<LineStripComponent>(entity);
        for(int i = 0; i < lineStripComponent.Points.size() - 1; i++)
        {
            DrawLine3D(lineStripComponent.Points[i], lineStripComponent.Points[i + 1], lineStripComponent.Color);
        }
    }
}
