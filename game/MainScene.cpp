#include <raylib.h>
#include "MainScene.h"
#include <rcamera.h>
#include "Player.h"
#include "../core/navigation/Navigation.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/util/Util.h"
#include "../core/ui/elements/slot/Slot.h"

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
