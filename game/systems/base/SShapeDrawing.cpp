#include "SShapeDrawing.h"
#include "entt/entt.hpp"
#include "primitives/Triangles.h"
#include "primitives/Polygon.h"
#include "components/CShapes.h"
#include "components/CPhysics.h"
namespace raylib
{
#include "raylib.h"
#include "raymath.h"
}
#include "util/raylibEx.h"
#include "components/CRay.h"

namespace tX
{

void SShapeDrawing::Draw()
{
    // DrawCubes
    for(const entt::entity& entity : Game::GetRegistry().view<CCube>())
    {
        const CCube& cubeComponent = Game::GetRegistry().get<CCube>(entity);
        raylib::DrawCube(ToRaylibVector3(cubeComponent.Position), cubeComponent.Size.x, cubeComponent.Size.y, cubeComponent.Size.z, ToRaylibColor(cubeComponent.Color));
    }

    // DrawCapsules
    for(const entt::entity& entity : Game::GetRegistry().view<CCapsule, CTransform>())
    {
        const CCapsule& capsuleComponent = Game::GetRegistry().get<CCapsule>(entity);
        const CTransform& transform = Game::GetRegistry().get<CTransform>(entity);
        v3 offset = v3{0.0f, capsuleComponent.Height / 2.0f, 0.0f};
        v3 top = transform.Position + offset;
        v3 bottom = transform.Position - offset;
        raylib::DrawCapsule(ToRaylibVector3(top), ToRaylibVector3(bottom),  capsuleComponent.Radius, 5, 3, ToRaylibColor(capsuleComponent.Color));
    }

    // Rays
    for(const entt::entity& entity : Game::GetRegistry().view<CRay>())
    {
        const CRay& rayComponent = Game::GetRegistry().get<CRay>(entity);
        raylib::Ray ray = {ToRaylibVector3(rayComponent.Ray.Origin), ToRaylibVector3(rayComponent.Ray.Direction)};
        raylib::DrawRay(ray, ToRaylibColor(rayComponent.Color));
    }

    // Draw Triangles
//    for(const entt::entity& entity : Game::GetRegistry().view<Triangle2D>())
//    {
//        const Triangle2D& triangleComponent = Game::GetRegistry().get<Triangle2D>(entity);
//        Triangle3D triangle3D{};
//        triangle3D.A = {triangleComponent.Vertices.A.x, 0.0f, triangleComponent.Vertices.A.y};
//        triangle3D.B = {triangleComponent.Vertices.B.x, 0.0f, triangleComponent.Vertices.B.y};
//        triangle3D.C = {triangleComponent.Vertices.C.x, 0.0f, triangleComponent.Vertices.C.y};
//
//        raylib::DrawTriangle3D(ToRaylibVector3(triangle3D.C), ToRaylibVector3(triangle3D.B), ToRaylibVector3(triangle3D.A), ToRaylibColor(triangleComponent.color));
//    }

    // Draw Linestrip
    for(const entt::entity& entity : Game::GetRegistry().view<CLineStrip>())
    {
        const CLineStrip& lineStripComponent = Game::GetRegistry().get<CLineStrip>(entity);
        for(int i = 0; i < lineStripComponent.Points.size() - 1; i++)
        {
            raylib::Vector3 v1 = {lineStripComponent.Points[i].x, 0.0f, lineStripComponent.Points[i].y};
            raylib::Vector3 v2 = {lineStripComponent.Points[i + 1].x, 0.0f, lineStripComponent.Points[i + 1].y};
            raylib::DrawLine3D(v1, v2, ToRaylibColor(lineStripComponent.Color));
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CMoveCircle>()) {
        CMoveCircle& mc = Game::GetRegistry().get<CMoveCircle>(entity);
        raylib::DrawCircle3D(ToRaylibVector3(mc.Position), mc.Radius, (raylib::Vector3){1.0f, 0.0f, 0.0f}, 90.0f, raylib::RED);
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CSphere, CTransform>())
    {
        const CSphere sphere = GetComponent<CSphere>(entity);
        raylib::DrawSphere(ToRaylibVector3(GetComponent<CTransform>(entity).Position), sphere.Radius, ToRaylibColor(sphere.Color));
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CPhysics2D, CTransform>())
    {
        auto physics = GetComponent<CPhysics2D>(entity);
        auto transform = GetComponent<CTransform>(entity);
        if(physics.CollisionType == ECollision2DType::Circle) {
            raylib::DrawCircle3D(ToRaylibVector3(transform.Position), physics.CollisionRadius, raylib::Vector3{1.0f, 0.0f, 0.0f}, 90.0f, raylib::BLUE);
        }
    }

    for(const entt::entity& entity : Game::GetRegistry().view<Polygon2D>())
    {
        const Polygon2D poly = GetComponent<Polygon2D>(entity);
        for(int i = 0; i < poly.Vertices.size() - 1; i++)
        {
            raylib::Vector3 v1 = {poly.Vertices[i].x, 0.0f, poly.Vertices[i].y};
            raylib::Vector3 v2 = {poly.Vertices[i + 1].x, 0.0f, poly.Vertices[i + 1].y};
            raylib::DrawLine3D(v1, v2, raylib::GREEN);
        }
    }
}

}
