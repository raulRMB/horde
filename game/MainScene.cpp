#include <raylib.h>
#include "MainScene.h"

#include "components/Components.h"
#include "Player.h"
#include "../core/util/Util.h"
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

Spawner spawnParticle = [](entt::entity e, Transform& transform, entt::registry& registry, ParticleComponent pc) {

    pc.MaxLife = 2;

    CapsuleComponent cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    Physics3DComponent phc = {};
    phc.Velocity = Util::RandVec3(-5, 5);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-2, 2);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<ParticleComponent>(e, pc);
    registry.emplace<CapsuleComponent>(e, cc);
    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, t);
};

Spawner spawnParticle2 = [](entt::entity e, Transform& transform, entt::registry& registry, ParticleComponent pc) {

    pc.MaxLife = 0.3;

    CapsuleComponent cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    Physics3DComponent phc = {};
    phc.Velocity = Util::RandVec3(-50, 50);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-20, 20);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<ParticleComponent>(e, pc);
    registry.emplace<CapsuleComponent>(e, cc);
    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, t);
};

void MainScene::HandleInput()
{
    pPlayer->HandleInput();

    if(IsKeyPressed(KEY_R))
    {
        RayCollision Collision = Util::GetMouseCollision();
        auto e = CreateEntity();
        AddComponent(e, EmitterComponent{.Frequency=0.1, .MaxParticles=100, .spawner=spawnParticle});
        AddComponent(e, Transform{Collision.point.x, 0.0f, Collision.point.z});
    }
    if(IsKeyPressed(KEY_T))
    {
        RayCollision Collision = Util::GetMouseCollision();
        auto e = CreateEntity();
        AddComponent(e, EmitterComponent{.Frequency=0.1, .MaxParticles=100, .spawner=spawnParticle2});
        AddComponent(e, Transform{Collision.point.x, 0.0f, Collision.point.z});
    }
}

void MainScene::Update(float deltaSeconds)
{
    mainCanvas->Update();
    Scene::Update(deltaSeconds);

    Camera3D& cam = Game::Instance().GetActiveCamera();
    Vector3 playerPos = pPlayer->GetComponent<Transform>().translation;
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