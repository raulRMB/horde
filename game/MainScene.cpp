#include <raylib.h>
#include "MainScene.h"
#include <rcamera.h>
#include "Player.h"
#include "../core/navigation/Navigation.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/util/Util.h"
#include "../core/ui/elements/slot/Slot.h"
#include "../core/Components.h"

MainScene::MainScene()
{


}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Scene::Start();
    InitUI();
    Player player{};
}

void MainScene::InitUI()
{
    mainCanvas = new Canvas();

    auto slot = new Slot({100, 100, 60, 60});
    auto slotImg = LoadTexture("assets/ui/images/concept.png");
    slot->SetImage(slotImg);
    mainCanvas->Add(slot);

    auto slot2 = new Slot({170, 100, 60, 60});
    auto slotImg2 = LoadTexture("assets/ui/images/map.png");
    slot2->SetImage(slotImg2);
    mainCanvas->Add(slot2);

    auto slot3 = new Slot({240, 100, 60, 60});
    auto slotImg3 = LoadTexture("assets/ui/images/arch.png");
    slot3->SetImage(slotImg3);
    mainCanvas->Add(slot3);

    auto slot4 = new Slot({310, 100, 60, 60});
    auto slotImg4 = LoadTexture("assets/ui/images/thresh.png");
    slot4->SetImage(slotImg4);
    mainCanvas->Add(slot4);
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

    if(IsKeyPressed(KEY_C))
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

    if(IsKeyPressed(KEY_F))
    {
        Tris = Navigation::BowyerWatson(Points);

        // for(const Navigation::TriangleNode& tri : Tris)
        // {
        //     auto e = CreateEntity();
        //     AddComponent(e, tri.GetTriangle());
        // }
    }
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
}

void MainScene::Load()
{
}
