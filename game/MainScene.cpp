#include <raylib.h>
#include "MainScene.h"

#include <fstream>
#include "../core/Components.h"
#include <sstream>

#include "Player.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/ui/elements/slot/Slot.h"
#include "../core/ui/elements/hotbar/Hotbar.h"
#include "../core/components/Components.h"
#include "../core/systems/NavigationSystem.h"
#include "../core/primitives/Triangles.h"

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

    auto hotbar = new Hotbar();
    hotbar->AddSlot("../assets/ui/images/concept.png");
    hotbar->AddSlot("../assets/ui/images/map.png");
    hotbar->AddSlot("../assets/ui/images/arch.png");
    hotbar->AddSlot("../assets/ui/images/thresh.png");    
    mainCanvas->Add(hotbar);
}

void MainScene::HandleInput()
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Ray ray = GetMouseRay(GetMousePosition(), Game::Instance().GetActiveCamera());
        Vector3 TopLeft = {-1000.0f, 0.0f, -1000.0f};
        Vector3 TopRight = {1000.0f, 0.0f, -1000.0f};
        Vector3 BottomLeft = {-1000.0f, 0.0f, 1000.0f};
        Vector3 BottomRight = {1000.0f, 0.0f, 1000.0f};

        RayCollision Collision = GetRayCollisionQuad(ray, TopRight, TopLeft , BottomLeft, BottomRight);

        auto e = CreateEntity();
        CapsuleComponent capsule{};
        capsule.Position = {Collision.point.x, 0.0f, Collision.point.z};
        capsule.Radius = 0.5f;
        capsule.Height = 1.0f;
        capsule.Color = RED;
        AddComponent(e, capsule);

        FollowComponent follow{};
        AddComponent(e, follow);

        Transform transform{};
        transform.translation = {Collision.point.x, 0.0f, Collision.point.z};
        AddComponent(e, transform);

        auto followEntities = Registry.view<FollowComponent, Transform>();

        for(const entt::entity& ent : followEntities)
        {
            std::vector<Navigation::TriangleNode*> path;
            FollowComponent& follow = GetComponent<FollowComponent>(ent);
            follow.bFollow = true;
            follow.Index = 1;

            StartPoint = {GetComponent<Transform>(ent).translation.x, GetComponent<Transform>(ent).translation.z};
            Navigation::AStar(StartPoint, EndPoint, path, Portals, Tris);

            std::vector<Vector2>& StringPath = follow.StringPath;

            StringPath = Navigation::StringPull(Portals, StartPoint, EndPoint);

            entt::entity e = CreateEntity();
            LineStripComponent lineStrip{};
            lineStrip.Points.reserve(StringPath.size());
            lineStrip.Color = GREEN;
            for(const Vector2& point : StringPath)
            {
                lineStrip.Points.push_back({point.x, 0.0f, point.y});
            }
            AddComponent(e, lineStrip);

            Portals.clear();

            follow.TargetPos = StringPath[follow.Index];
        }
    }

    const Vector3 mouseWorldPosition = Util::GetMouseWorldPosition();
    if(IsKeyPressed(KEY_C))
    {
        auto e = CreateEntity();
        CapsuleComponent capsule{};
        capsule.Position = {mouseWorldPosition.x, 0.0f, mouseWorldPosition.z};
        capsule.Color = RED;
        capsule.Radius = 0.5f;
        capsule.Height = 1.0f;
        AddComponent(e, capsule);

        Points.push_back({Collision.point.x, Collision.point.z});

    }
    if(IsKeyPressed(KEY_X))
    {
        Points.push_back({-3.0f, -3.0f});
        Points.push_back({0.0f, 3.0f});
        Points.push_back({3.0f, -3.0f});
    }

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

        Points.push_back({mouseWorldPosition.x, mouseWorldPosition.z});
    }
    static bool bCursorEnabled = true;
    if(IsKeyPressed(KEY_A))
    {
        if(bCursorEnabled)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
        bCursorEnabled = !bCursorEnabled;
    }
    if(IsKeyPressed(KEY_NINE))
    {
        Save();
    }
    if(IsKeyPressed(KEY_ZERO))
    {
        Load();
    }

    pPlayer->HandleInput();
}

void MainScene::Update(float deltaSeconds)
{
    mainCanvas->Update();
    Scene::Update(deltaSeconds);
    UpdateCamera(&Game::Instance().GetActiveCamera(), CAMERA_FIRST_PERSON);
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
}

void MainScene::Save()
{
    System::Get<NavigationSystem>().SaveNavMesh();
}

void MainScene::Load()
{
    System::Get<NavigationSystem>().LoadNavMesh();
}