#include "MainScene.h"
#include "components/CTransform.h"
#include "ui/elements/slot/Slot.h"
#include "ui/elements/hotbar/Hotbar.h"
#include "systems/moba/SNavigation.h"
#include "abilities/Projectile.h"
#include "components/CModel.h"
#include "util/Util.h"
#include "networking/base/NetworkDriver.h"
#include "primitives/RayCollision.h"
#include "util/Builder.h"
#include "components/CFollow.h"
#include "systems/base/SAnimation.h"
#include "systems/base/SCharacterAnimation.h"
#include "systems/moba/SFollow.h"
#include "systems/base/SPhysics.h"
#include "systems/base/SShapeDrawing.h"
#include "systems/base/SParticle.h"
#include "systems/base/SNetworking.h"
#include "systems/moba/SNavigation.h"
#include "systems/base/SModelDrawing.h"
#include "systems/moba/SAttribute.h"
#include "systems/moba/Spawn.h"
#include "systems/moba/SAttributeUI.h"
#include "systems/base/SParticleDrawing.h"
#include "systems/base/SLifetime.h"
#include "systems/SEnemy.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "renderer/Renderer.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "tiny_gltf.h"

namespace tX
{

MainScene::MainScene(){}

MainScene::~MainScene() = default;

void MainScene::LoadSystems()
{
    UpdateSystems.push_back(&System::Get<SLifetime>());
    UpdateSystems.push_back(&System::Get<SPhysics>());

    if(!Game::IsStandalone()) {
        UpdateSystems.push_back(&System::Get<SNetworking>());
    }

    if(!Game::IsServer()) {
        DrawingSystems.push_back(&System::Get<SModelDrawing>());
        DrawingSystems.push_back(&System::Get<SShapeDrawing>());
        UpdateSystems.push_back(&System::Get<SAnimation>());
        UpdateSystems.push_back(&System::Get<SParticle>());
        DrawingSystems.push_back(&System::Get<SParticleDrawing>());
        DrawingSystems.push_back(&System::Get<SAttributeUI>());
    }

    if(!Game::IsClient()) {
        UpdateSystems.push_back(&System::Get<SEnemy>());
        UpdateSystems.push_back(&System::Get<SNavigation>());
        UpdateSystems.push_back(&System::Get<SFollow>());
        UpdateSystems.push_back(&System::Get<SSpawn>());
        UpdateSystems.push_back(&System::Get<SAttribute>());
        UpdateSystems.push_back(&System::Get<SCharacterAnimation>());
    }
}

struct PosNormalVertex
{
    float x, y, z;
    float nx, ny, nz;
};

bgfx::VertexBufferHandle vbh;
bgfx::IndexBufferHandle ibh;

void MainScene::Start()
{
    LoadSystems();
    Load();

    if(!Game::IsClient()) {
        Builder::Spawner();
    }

    if(!Game::IsServer())
    {
        InitUI();
        Builder::Map();

        CCamera3D& cam = Game::Instance().GetActiveCamera();
        const v3& pos = v3{0,0,0};
        cam.Position = v3{pos.x - 40, cam.Position.y, pos.z - 40};
        cam.Target = pos;
    }
    if(Game::IsStandalone()) {
        CTransform t = CTransform{};
        t.Scale = v3{0.055};
        Game::SpawnPlayer(1, t, true);
    }
    Scene::Start();

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, "../assets/models/anim.glb");
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }
    else
    {
        printf("Loaded glTF\n");
    }

//    // Extract vertex and index data from glTF
//    std::vector<PosNormalVertex> vertices;
//    std::vector<uint16_t> indices;
//
//    for (const auto& mesh : model.meshes) {
//        for (const auto& primitive : mesh.primitives) {
//            const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
//            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
//            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
//
//            const void* indexData = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
//            const uint32_t indexCount = static_cast<uint32_t>(accessor.count);
//
//            indices.resize(indexCount);
//            memcpy(indices.data(), indexData, indexCount * sizeof(uint16_t));
//
//            const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
//            const tinygltf::BufferView& positionView = model.bufferViews[positionAccessor.bufferView];
//            const tinygltf::Buffer& positionBuffer = model.buffers[positionView.buffer];
//
//            const void* positionData = &positionBuffer.data[positionView.byteOffset + positionAccessor.byteOffset];
//            const uint32_t vertexCount = static_cast<uint32_t>(positionAccessor.count);
//
//            vertices.resize(vertexCount);
//            memcpy(vertices.data(), positionData, vertexCount * sizeof(PosNormalVertex));
//        }
//    }
//
//    // Create BGFX buffers
//    vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices.data(), vertices.size() * sizeof(PosNormalVertex)),  bgfx::Attrib::Position | bgfx::Attrib::Normal);
//
//    ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices.data(), indices.size() * sizeof(uint16_t)));
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
//    if(raylib::IsKeyPressed(raylib::KEY_Y)) {
//        cameraLock = !cameraLock;
//    }
//    if(!cameraLock) {
//        CCamera3D& cam = Game::Instance().GetActiveCamera();
//        v3 upDown = v3{0.1, 0, 0.1};
//        v3 leftRight = v3{0.1, 0, -0.1};
//        if (raylib::IsKeyDown(raylib::KEY_UP)) {
//            cam.Position += upDown;
//            cam.Target += upDown;
//        }
//        if (raylib::IsKeyDown(raylib::KEY_DOWN)) {
//            cam.Position -= upDown;
//            cam.Target -= upDown;
//        }
//        if (raylib::IsKeyDown(raylib::KEY_LEFT)) {
//            cam.Position += leftRight;
//            cam.Target += leftRight;
//        }
//        if (raylib::IsKeyDown(raylib::KEY_RIGHT)) {
//            cam.Position -= leftRight;
//            cam.Target -= leftRight;
//        }
//    }
//    if(raylib::IsMouseButtonDown(raylib::MOUSE_BUTTON_RIGHT)) {
//        v2 mousePos = Util::GetMouseWorldPosition2D();
//        if(System::Get<SNavigation>().IsValidPoint(mousePos))
//        {
//            if(Game::IsClient()) {
//                auto NetId = NetworkDriver::GetNetworkedEntities().Get(player);
//                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
//            } else {
//                CFollow &followComponent = Game::GetRegistry().get<CFollow>(GetActivePlayer());
//                followComponent.FollowState = EFollowState::Dirty;
//                followComponent.Index = 1;
//                followComponent.Goal = mousePos;
//            }
//        }
//    }
//    if(raylib::IsMouseButtonPressed(raylib::MOUSE_BUTTON_RIGHT))
//    {
//        auto e = Game::GetRegistry().create();
//        CMoveCircle mc = CMoveCircle {};
//        mc.Position = Util::GetMouseWorldPosition();
//        mc.Radius = 2.2;
//
//        Game::GetRegistry().emplace<CMoveCircle>(e, mc);
//        v2 mousePos = Util::GetMouseWorldPosition2D();
//        if(System::Get<SNavigation>().IsValidPoint(mousePos))
//        {
//            if(Game::IsClient()) {
//                auto NetId = NetworkDriver::GetNetworkedEntities().Get(player);
//                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
//            } else {
//                CFollow &followComponent = Game::GetRegistry().get<CFollow>(GetActivePlayer());
//                followComponent.FollowState = EFollowState::Dirty;
//                followComponent.Index = 1;
//                followComponent.Goal = mousePos;
//            }
//        }
//    }
//    if(raylib::IsKeyPressed(raylib::KEY_Q))
//    {
//        FRayCollision Collision = Util::GetMouseCollision();
//        auto vec = v3(Collision.point.x, 0.0f, Collision.point.z);
//        if(Game::IsClient()) {
//            auto netId = NetworkDriver::GetNetworkedEntities().Get(Game::GetPlayer());
//            NetworkDriver::GetClient()->TriggerAbility(netId, 0, vec);
//        } else {
//            player = Game::GetPlayer();
//            entt::registry& registry = Game::GetRegistry();
//            CTransform clickPoint = CTransform{vec};
//            CTransform t = registry.get<CTransform>(player);
//            Projectile(player, clickPoint.Position, t.Position, 50, 1.2);
//        }
//    }
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
        if(cameraLock && Game::GetPlayer() != entt::null) {
            const v3& playerPos = GetRegistry().get<CTransform>(player).Position;
            cam.Position = glm::mix(cam.Position, v3{playerPos.x - 40, cam.Position.y, playerPos.z - 40}, 0.05);
            cam.Target = glm::mix(cam.Target, playerPos, 0.05);
        }
    }
}

int counter = 0;

void MainScene::Draw()
{
    Scene::Draw();

    const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
    float view[16];
    bx::mtxLookAt(view, eye, at);
    float proj[16];
    bx::mtxProj(proj, 60.0f, float(Renderer::GetWidth()) / float(Renderer::GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    float mtx[16];
    bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
    bgfx::setTransform(mtx);

//    for(const entt::entity& entity : Registry.view<raylib::Model, CTransform>())
//    {
//        const raylib::Model& model = Registry.get<raylib::Model>(entity);
//        const CTransform& transform = Registry.get<CTransform>(entity);
//        raylib::DrawModel(model, ToRaylibVector3(transform.Position), 0.1f, raylib::WHITE);
//    }
    counter++;
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
