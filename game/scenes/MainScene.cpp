#include "raylib.h"
#include "MainScene.h"

#include "components/Components.h"
#include "Player.h"
#include "util/Util.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/NavigationSystem.h"

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
    p2Player = new Player();
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
    GetActivePlayer()->HandleInput(&Registry);

    if(IsKeyPressed(KEY_SPACE)) {
        p1Active = !p1Active;
    }

}

Player* MainScene::GetActivePlayer() {
    return p1Active ? pPlayer : p2Player;
}

void MainScene::Update(float deltaSeconds)
{
    mainCanvas->Update();
    Scene::Update(deltaSeconds);

    Camera3D& cam = Game::Instance().GetActiveCamera();
    Vector3 playerPos = GetActivePlayer()->GetComponent<Transform>().translation;
    cam.position = Vector3{playerPos.x - 20, cam.position.y, playerPos.z - 40};
    cam.target = playerPos;
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
    pPlayer->DrawUI();
    p2Player->DrawUI();
    mainCanvas->Draw();
}

void MainScene::Clean()
{
    delete pPlayer;
    delete p2Player;
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