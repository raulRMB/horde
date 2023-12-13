#include "MainScene.h"
#include "smartentity/Player.h"
#include "components/Transform.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/Navigation.h"
#include "util/raylibEx.h"
#include "abilities/Projectile.h"
#include "components/Attribute.h"
#include "components/Model.h"
#include "components/Physics.h"
#include "util/Util.h"

namespace tZ
{
    struct CEmitter;

MainScene::MainScene()
{

}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Load();
    if(!Game::IsServer())
    {
        InitUI();

        entt::entity e = CreateEntity();
        CTransform t = {};
        t.Rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
        t.Scale = v3(250.f);
        AddComponent(e, t);
        raylib::Model m = raylib::LoadModelFromMesh(raylib::GenMeshPlane(1, 1, 1, 1));
        m.materials[0].maps[raylib::MATERIAL_MAP_DIFFUSE].texture = raylib::LoadTexture("../assets/textures/arena.png");
        AddComponent(e, CModel{m, 1.f, false});
        CCamera3D& cam = Game::Instance().GetActiveCamera();
        const v3& pos = v3{0,0,0};
        cam.Position = v3{pos.x - 40, cam.Position.y, pos.z - 40};
        cam.Target = pos;
    }
    Scene::Start();
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
    if(raylib::IsKeyPressed(raylib::KEY_Y)) {
        cameraLock = !cameraLock;
    }
    if(!cameraLock) {
        CCamera3D& cam = Game::Instance().GetActiveCamera();
        v3 upDown = v3{0.1, 0, 0.1};
        v3 leftRight = v3{0.1, 0, -0.1};
        if (raylib::IsKeyDown(raylib::KEY_UP)) {
            cam.Position += upDown;
            cam.Target += upDown;
        }
        if (raylib::IsKeyDown(raylib::KEY_DOWN)) {
            cam.Position -= upDown;
            cam.Target -= upDown;
        }
        if (raylib::IsKeyDown(raylib::KEY_LEFT)) {
            cam.Position += leftRight;
            cam.Target += leftRight;
        }
        if (raylib::IsKeyDown(raylib::KEY_RIGHT)) {
            cam.Position -= leftRight;
            cam.Target -= leftRight;
        }
    }
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
        if(player != nullptr && cameraLock) {
            const v3& playerPos = player->GetComponent<CTransform>().Position;
            cam.Position = glm::mix(cam.Position, v3{playerPos.x - 40, cam.Position.y, playerPos.z - 40}, 0.05);
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
