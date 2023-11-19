#include <raylib.h>
#include "MainScene.h"

#include "components/Components.h"
#include "Player.h"
#include "../core/ui/elements/slot/Slot.h"
#include "../core/ui/elements/hotbar/Hotbar.h"
#include "../core/systems/NavigationSystem.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Load();
    Scene::Start();
    InitUI();
    pPlayer = new Player();
}

void MainScene::InitUI()
{
    mainCanvas = new Canvas();

    Hotbar* hotbar = new Hotbar();
    hotbar->AddSlot("../assets/ui/images/concept.png");
    hotbar->AddSlot("../assets/ui/images/map.png");
    hotbar->AddSlot("../assets/ui/images/arch.png");
    hotbar->AddSlot("../assets/ui/images/thresh.png");    
    mainCanvas->Add(hotbar);
}

void MainScene::HandleInput()
{
    pPlayer->HandleInput();

    if(IsKeyPressed(KEY_R))
    {
        Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
        Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
        Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
        Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
        Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};
        RayCollision Collision = GetRayCollisionQuad(ray, TopRight, TopLeft , BottomLeft, BottomRight);
        auto e = CreateEntity();
        auto sc = CubeComponent{};
        sc.Color = RAYWHITE;
        sc.Position = Vector3{0,0,0};
        sc.Size = Vector3{1, 1, 1};
        AddComponent(e, EmitterComponent{.Frequency=0.1, .MaxParticles=100});
        AddComponent(e, Transform{Collision.point.x, 0.0f, Collision.point.z});
        AddComponent(e, sc);
    }
}

void MainScene::Update(float deltaSeconds)
{
    mainCanvas->Update();
    Scene::Update(deltaSeconds);
}

void MainScene::Draw()
{
    Scene::Draw();
    DrawGrid(30, 1.0f);
    for(const entt::entity& entity : Registry.view<Model, Transform>())
    {
        const Model& model = Registry.get<Model>(entity);
        const Transform& transform = Registry.get<Transform>(entity);
        DrawModel(model, transform.translation, 0.1f, WHITE);
    }
}

void MainScene::DrawUI()
{
    mainCanvas->Draw();
}

void MainScene::Clean()
{
    delete pPlayer;
    delete mainCanvas;
    Scene::Clean();
}

void MainScene::Save()
{
    System::Get<NavigationSystem>().SaveNavMesh();
}

void MainScene::Load()
{
    System::Get<NavigationSystem>().LoadNavMesh();
}