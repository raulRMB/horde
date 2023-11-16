#include <raylib.h>
#include "MainScene.h"

#include <rcamera.h>

#include "Player.h"
#include "../core/navigation/Navigation.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/util/Util.h"

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
    mainCanvas->Add(new Button({100, 100, 100, 50}, "1"));
    mainCanvas->Add(new Button({205, 100, 100, 50}, "2"));
    mainCanvas->Add(new Button({310, 100, 100, 50}, "3"));

    mainCanvas->Add(new Button({100, 155, 100, 50}, "4"));
    mainCanvas->Add(new Button({205, 155, 100, 50}, "5"));
    mainCanvas->Add(new Button({310, 155, 100, 50}, "6"));

    mainCanvas->Add(new Button({100, 210, 100, 50}, "7"));
    mainCanvas->Add(new Button({205, 210, 100, 50}, "8"));
    mainCanvas->Add(new Button({310, 210, 100, 50}, "9"));
}

void MainScene::HandleInput()
{
    IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    Vector3 start = GetCameraForward(&Game::Instance().GetActiveCamera());
    Vector3 end = GetMouseWorldPosition();
    Vector3 p = Navigation::Intersect({}, {0.0f, 1.0f, 0.0f}, start, end - start);
    EndPoint = {p.x, p.z};

    auto e = CreateEntity();
    Transform transform{};
    transform.translation = {p.x, 0.0f, p.z};

    Cylinder
    AddComponent(e, transform);
    AddComponent(e, CLineMesh(1));

    for(const entt::entity& ent : FollowEntities)
    {
        std::vector<Navigation::TriangleNode*> path;
        CFollow& follow = GetComponent<CFollow>(ent);
        follow.bFollow = true;
        follow.index = 1;

        StartPoint = {GetComponent<CTransform3d>(ent).WorldPosition.x, GetComponent<CTransform3d>(ent).WorldPosition.z};
        Navigation::AStar(StartPoint, EndPoint, path, Portals, Tris);

        std::vector<v2>& StringPath = follow.StringPath;

        StringPath = Navigation::StringPull(Portals, StartPoint, EndPoint);
        Portals.clear();

        follow.TargetPos = StringPath[follow.index];
    }
}

void MainScene::Update(float deltaSeconds)
{
    mainCanvas->Update();
    Scene::Update(deltaSeconds);
}

void MainScene::Draw()
{
    Scene::Draw();
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
