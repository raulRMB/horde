#include "MainScene.h"
#include "Player.h"
#include "components/Transform.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/Navigation.h"
#include "raylibEx.h"

namespace tZ
{

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
        CCamera3D& cam = Game::Instance().GetActiveCamera();
        Player* player = GetActivePlayer();
        if(player != nullptr) {
            const v3& playerPos = player->GetComponent<CTransform>().Position;
            cam.Position = glm::mix(cam.Position, v3{playerPos.x - 20, cam.Position.y, playerPos.z - 40}, 0.05);
            cam.Target = glm::mix(cam.Target, playerPos, 0.05);
        }
    }
}

void MainScene::Draw()
{
    Scene::Draw();
    for(const entt::entity& entity : Registry.view<raylib::Model, CTransform>())
    {
        const raylib::Model& model = Registry.get<raylib::Model>(entity);
        const CTransform& transform = Registry.get<CTransform>(entity);
        raylib::DrawModel(model, ToRaylibVector3(transform.Position), 0.1f, raylib::WHITE);
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

}
