//
// Created by raul on 21/11/23.
//

#include "TestScene.h"
#include "primitives/Triangles.h"
#include "util/Util.h"
#include "components/Shapes.h"
#include "primitives/Polygon.h"
#include <raylib.h>

namespace tZ
{

std::vector<Triangle2D> triangles{};
Triangle2D triangle{};
Polygon2D polygon{};


void TestScene::Start()
{
    Scene::Start();

    entt::entity e = CreateEntity();
    triangle.Vertices.A = {0.f, 10.f};
    triangle.Vertices.B = {-10.f, 0.f};
    triangle.Vertices.C = {10.f, 0.f};
    triangle.color = Red;
    AddComponent(e, triangle);
    triangles.push_back(triangle);

    e = CreateEntity();
    triangle.Vertices.C = {0.f, -10.f};
    triangle.Vertices.B = {-10.f, 0.f};
    triangle.Vertices.A = {10.f, 0.f};
    triangle.color = Red;
    AddComponent(e, triangle);
    triangles.push_back(triangle);

    polygon.Vertices.push_back({-10.f, 0.f});
    polygon.Vertices.push_back({0.f, 30.f});
    polygon.Vertices.push_back({10.f, 5.f});
    polygon.Vertices.push_back({0.f, -10.f});
    polygon.Vertices.push_back({-10.f, 0.f});
    e = CreateEntity();
    AddComponent(e, polygon);
}

void TestScene::Update(float deltaSeconds)
{
    Scene::Update(deltaSeconds);
}

void TestScene::DrawUI()
{

}

void TestScene::Draw()
{
    Scene::Draw();
}

void TestScene::Clean()
{
    Scene::Clean();
}

void TestScene::HandleInput()
{
    if(IsKeyDown(KEY_A))
    {
        v2 pos = Util::GetMouseWorldPosition2D();

        entt::entity e = CreateEntity();
        CSphere circle{};
        circle.Radius = 1.f;
        circle.Color = Util::RandColor();
        AddComponent(e, circle);
//        for(const entt::entity& entity : Game::GetRegistry().view<Triangle2D>())
//        {
//            const Triangle2D& triangle = Game::GetRegistry().get<Triangle2D>(entity);
//            if(Util::IsPointInsideTriangle(triangle, pos))
//            {
//                Transform t{};
//                t.translation = {pos.x, 0.f, pos.y};
//                AddComponent(e, t);
//                break;
//            }
//        }
        pos -= Util::MinimumTranslationVector(polygon, pos);
        Transform t{};
        t.translation = {pos.x, 0.f, pos.y};
        AddComponent(e, t);
    }
}

}
