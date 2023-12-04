#include "ShapeDrawing.h"
#include "entt/entt.hpp"
#include "primitives/Triangles.h"
#include "primitives/Polygon.h"
#include "components/Shapes.h"
#include "components/Physics.h"
#include <raylib.h>

#include "raylibEx.h"
#include "raymath.h"
#include "components/Ray.h"

namespace tZ
{

void SShapeDrawing::Draw()
{
    // DrawCubes
    for(const entt::entity& entity : Game::GetRegistry().view<CCube>())
    {
        const CCube& cubeComponent = Game::GetRegistry().get<CCube>(entity);
        DrawCube(ToRaylibVector3(cubeComponent.Position), cubeComponent.Size.x, cubeComponent.Size.y, cubeComponent.Size.z, ToRaylibColor(cubeComponent.Color));
    }

    // DrawCapsules
    for(const entt::entity& entity : Game::GetRegistry().view<CCapsule, Transform>())
    {
        const CCapsule& capsuleComponent = Game::GetRegistry().get<CCapsule>(entity);
        const Transform& transform = Game::GetRegistry().get<Transform>(entity);
        Vector3 offset = {0.0f, capsuleComponent.Height / 2.0f, 0.0f};
        Vector3 top = Vector3Add(transform.translation, offset);
        Vector3 bottom = Vector3Subtract(transform.translation, offset);
        DrawCapsule(top, bottom,  capsuleComponent.Radius, 5, 3, ToRaylibColor(capsuleComponent.Color));
    }

    // Rays
    for(const entt::entity& entity : Game::GetRegistry().view<CRay>())
    {
        const CRay& rayComponent = Game::GetRegistry().get<CRay>(entity);
        Ray ray = {ToRaylibVector3(rayComponent.Ray.Origin), ToRaylibVector3(rayComponent.Ray.Direction)};
        DrawRay(ray, ToRaylibColor(rayComponent.Color));
    }

    // Draw Triangles
    for(const entt::entity& entity : Game::GetRegistry().view<Triangle2D>())
    {
        const Triangle2D& triangleComponent = Game::GetRegistry().get<Triangle2D>(entity);
        Triangle3D triangle3D{};
        triangle3D.A = {triangleComponent.Vertices.A.x, 0.0f, triangleComponent.Vertices.A.y};
        triangle3D.B = {triangleComponent.Vertices.B.x, 0.0f, triangleComponent.Vertices.B.y};
        triangle3D.C = {triangleComponent.Vertices.C.x, 0.0f, triangleComponent.Vertices.C.y};

        DrawTriangle3D(ToRaylibVector3(triangle3D.C), ToRaylibVector3(triangle3D.B), ToRaylibVector3(triangle3D.A), ToRaylibColor(triangleComponent.color));
    }

    // Draw Linestrip
    for(const entt::entity& entity : Game::GetRegistry().view<CLineStrip>())
    {
        const CLineStrip& lineStripComponent = Game::GetRegistry().get<CLineStrip>(entity);
        for(int i = 0; i < lineStripComponent.Points.size() - 1; i++)
        {
            Vector3 v1 = {lineStripComponent.Points[i].x, 0.0f, lineStripComponent.Points[i].y};
            Vector3 v2 = {lineStripComponent.Points[i + 1].x, 0.0f, lineStripComponent.Points[i + 1].y};
            DrawLine3D(v1, v2, ToRaylibColor(lineStripComponent.Color));
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CSphere, Transform>())
    {
        const CSphere sphere = GetComponent<CSphere>(entity);
        DrawSphere(ToRaylibVector3(GetComponent<CTransform>(entity).Position), sphere.Radius, ToRaylibColor(sphere.Color));
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CPhysics2D, Transform>())
    {
        auto physics = GetComponent<CPhysics2D>(entity);
        auto transform = GetComponent<CTransform>(entity);
        if(physics.CollisionType == ECollision2DType::Circle) {
            DrawCircle3D(ToRaylibVector3(transform.Position), physics.CollisionRadius, Vector3{1.0f, 0.0f, 0.0f}, 90.0f, BLUE);
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

}
