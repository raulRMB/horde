#include "MainScene.h"
#include "components/CTransform.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/SNavigation.h"
#include "util/raylibEx.h"
#include "abilities/Projectile.h"
#include "components/CModel.h"
#include "util/Util.h"
#include "networking/base/NetworkDriver.h"
#include "primitives/RayCollision.h"
#include "util/Builder.h"

namespace tX
{

MainScene::MainScene(){}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Load();

    if(!Game::IsServer())
    {
        Builder::Spawner();
        InitUI();
        Builder::Map();

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
    entt::entity player = GetActivePlayer();
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
    if(raylib::IsMouseButtonDown(raylib::MOUSE_BUTTON_RIGHT)) {
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(player);
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsMouseButtonPressed(raylib::MOUSE_BUTTON_RIGHT))
    {
        auto e = Game::GetRegistry().create();
        CMoveCircle mc = CMoveCircle {};
        mc.Position = Util::GetMouseWorldPosition();
        mc.Radius = 2.2;

        Game::GetRegistry().emplace<CMoveCircle>(e, mc);
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(player);
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsKeyPressed(raylib::KEY_Q))
    {
            FRayCollision Collision = Util::GetMouseCollision();
            auto vec = v3(Collision.point.x, 0.0f, Collision.point.z);
            auto netId = NetworkDriver::GetNetworkedEntities().Get(Game::GetPlayer());
            NetworkDriver::GetClient()->TriggerAbility(netId, 0, vec);
//            CTransform clickPoint = CTransform{vec};
//            CTransform t = GetComponent<CTransform>();
//            Projectile(GetEntity(), clickPoint.Position, t.Position);
    }
}

entt::entity MainScene::GetActivePlayer()
{
    return Game::GetPlayer();
}

void MainScene::Update(float deltaSeconds)
{
    Scene::Update(deltaSeconds);
    if(!Game::IsServer()) {
        mainCanvas->Update();
        CCamera3D& cam = Game::Instance().GetActiveCamera();
        entt::entity player = GetActivePlayer();
        if(cameraLock) {
            const v3& playerPos = GetRegistry().get<CTransform>(player).Position;
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
    System::Get<SNavigation>().LoadNavMesh(Game::GetRegistry());
}

}
