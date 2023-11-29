#include "ShapeDrawing.h"
#include "entt/entt.hpp"
#include "raymath.h"
#include "primitives/Triangles.h"
#include "primitives/Polygon.h"
#include "components/Shapes.h"
#include "components/Physics.h"

void SShapeDrawing::Draw()
{
    // DrawCubes
    for(const entt::entity& entity : Game::GetRegistry().view<CCube>())
    {
        const CCube& cubeComponent = Game::GetRegistry().get<CCube>(entity);
        DrawCube(cubeComponent.Position, cubeComponent.Size.x, cubeComponent.Size.y, cubeComponent.Size.z, cubeComponent.color);
    }

    // DrawCapsules
    for(const entt::entity& entity : Game::GetRegistry().view<CCapsule, Transform>())
    {
        const CCapsule& capsuleComponent = Game::GetRegistry().get<CCapsule>(entity);
        const Transform& transform = Game::GetRegistry().get<Transform>(entity);
        Vector3 offset = {0.0f, capsuleComponent.Height / 2.0f, 0.0f};
        Vector3 top = Vector3Add(transform.translation, offset);
        Vector3 bottom = Vector3Subtract(transform.translation, offset);
        DrawCapsule(top, bottom,  capsuleComponent.Radius, 5, 3, capsuleComponent.color);
    }

    // Rays
    for(const entt::entity& entity : Game::GetRegistry().view<CRay>())
    {
        const CRay& rayComponent = Game::GetRegistry().get<CRay>(entity);
        DrawRay(rayComponent.ray, rayComponent.color);
    }

    // Draw Triangles
    for(const entt::entity& entity : Game::GetRegistry().view<Triangle2D>())
    {
        const Triangle2D& triangleComponent = Game::GetRegistry().get<Triangle2D>(entity);
        Triangle3D triangle3D{};
        triangle3D.A = {triangleComponent.Vertices.A.x, 0.0f, triangleComponent.Vertices.A.y};
        triangle3D.B = {triangleComponent.Vertices.B.x, 0.0f, triangleComponent.Vertices.B.y};
        triangle3D.C = {triangleComponent.Vertices.C.x, 0.0f, triangleComponent.Vertices.C.y};
        DrawTriangle3D(triangle3D.C, triangle3D.B, triangle3D.A, triangleComponent.color);
    }

    // Draw Linestrip
    for(const entt::entity& entity : Game::GetRegistry().view<CLineStrip>())
    {
        const CLineStrip& lineStripComponent = Game::GetRegistry().get<CLineStrip>(entity);
        for(int i = 0; i < lineStripComponent.Points.size() - 1; i++)
        {
            DrawLine3D(lineStripComponent.Points[i], lineStripComponent.Points[i + 1], lineStripComponent.color);
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CSphere, Transform>())
    {
        const CSphere sphere = GetComponent<CSphere>(entity);
        DrawSphere(GetComponent<Transform>(entity).translation, sphere.Radius, sphere.color);   
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CPhysics2D, Transform>())
    {
        auto physics = GetComponent<CPhysics2D>(entity);
        auto transform = GetComponent<Transform>(entity);
        if(physics.CollisionType == ECollision2DType::Circle) {
            DrawCircle3D(transform.translation, physics.CollisionRadius, Vector3{1.0f, 0.0f, 0.0f}, 90.0f, BLUE);
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<Polygon2D>())
    {
        const Polygon2D poly = GetComponent<Polygon2D>(entity);
        for(int i = 0; i < poly.Vertices.size() - 1; i++)
        {
            Vector3 v1 = {poly.Vertices[i].x, 0.0f, poly.Vertices[i].y};
            Vector3 v2 = {poly.Vertices[i + 1].x, 0.0f, poly.Vertices[i + 1].y};
            DrawLine3D(v1, v2, GREEN);
        }
    }
}
