#include <raylib.h>
#include "MainScene.h"

#include <fstream>
#include "../core/Components.h"
#include <sstream>

#include "Player.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/ui/elements/slot/Slot.h"
#include "../core/ui/elements/hotbar/Hotbar.h"


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
    std::ofstream file;
    file.open("../assets/save.txt");
    for(const Vector2& point : Points)
    {
        file << point.x << " " << point.y << "\n";
    }

    file << "TRIANGLES\n";

    for(const Navigation::TriangleNode& graphTriangle : Tris)
    {
        file << graphTriangle.GetIndex() << " ";
        file << graphTriangle.IsBlocked() << "\n";
    }

    file.close();
}

void MainScene::Load()
{
    std::ifstream file;
    file.open("../assets/save.txt");
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line) && line != "TRIANGLES")
        {
            std::stringstream ss(line);
            float x, y;
            ss >> x >> y;
            Points.push_back({x, y});
        }

        Tris = Navigation::BowyerWatson(Points);

        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            unsigned int idx;
            bool blocked;
            ss >> idx >> blocked;
            Tris[idx].SetBlocked(blocked);
        }

        file.close();

        for(const Navigation::TriangleNode& graphTriangle : Tris)
        {
            auto e = CreateEntity();
            AddComponent(e, Transform());
            if(graphTriangle.IsBlocked())
            {
                Triangle2D triangle = graphTriangle.GetTriangle();
                triangle.Color = RED;
                AddComponent(e, triangle);
            }
        }
    }
}