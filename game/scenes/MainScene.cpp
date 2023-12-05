#include "raylib.h"
#include "MainScene.h"

#include "Player.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/Navigation.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Load();
    Scene::Start();
    if(!Game::IsServer()) {
        InitUI();
    }
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
    Player* player = GetActivePlayer();
    if(player != nullptr) {
        player->HandleInput(&Registry);
    }
}

Player* MainScene::GetActivePlayer() {
    return Game::GetPlayer();
}

void MainScene::Update(float deltaSeconds)
{

    Scene::Update(deltaSeconds);
    if(!Game::IsServer()) {
        mainCanvas->Update();
        Camera3D &cam = Game::Instance().GetActiveCamera();
        Player* player = GetActivePlayer();
        if(player != nullptr) {
            Vector3 playerPos = player->GetComponent<Transform>().translation;
            cam.position = Vector3{playerPos.x - 20, cam.position.y, playerPos.z - 40};
            cam.target = playerPos;
        }
    }
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
    Scene::DrawUI();
    Player* player = Game::GetPlayer();
    if(player != nullptr) {
        player->DrawUI();
    }
    mainCanvas->Draw();
}

void MainScene::Clean()
{
    delete mainCanvas;
    Scene::Clean();
}

void MainScene::Save()
{
    System::Get<SNavigation>().SaveNavMesh();
}

void MainScene::Load()
{
    System::Get<SNavigation>().LoadNavMesh();
}
