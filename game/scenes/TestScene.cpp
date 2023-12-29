#include "TestScene.h"
#include "primitives/Triangles.h"
#include "util/Util.h"
#include "components/CShapes.h"
#include "primitives/Polygon.h"
#include "components/CTransform.h"

namespace tX
{

std::vector<CTriangle2D> triangles{};
CTriangle2D triangle{};
Polygon2D polygon{};


void TestScene::Start()
{
    Scene::Start();

    entt::entity e = CreateEntity();
    triangle.Vertices.A = {0.f, 10.f};
    triangle.Vertices.B = {-10.f, 0.f};
    triangle.Vertices.C = {10.f, 0.f};
    triangle.Color = FColor(0xFF0000FF);
    AddComponent(e, triangle);
    triangles.push_back(triangle);

    e = CreateEntity();
    triangle.Vertices.C = {0.f, -10.f};
    triangle.Vertices.B = {-10.f, 0.f};
    triangle.Vertices.A = {10.f, 0.f};
    triangle.Color = FColor(0xFF0000FF);
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
}

}
