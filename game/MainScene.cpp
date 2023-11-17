#include <raylib.h>
#include "MainScene.h"

#include <fstream>
#include <rcamera.h>
#include <sstream>

#include "Player.h"
#include "../core/navigation/Navigation.h"
#include "../core/ui/elements/button/Button.h"
#include "../core/util/Util.h"
#include "../core/ui/elements/slot/Slot.h"
#include "../core/Components.h"
#include "../core/systems/NavigationSystem.h"

MainScene::MainScene()
{

}

MainScene::~MainScene() = default;

void MainScene::Start()
{
    Scene::Start();
    InitUI();
    pPlayer = new Player();
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

    for(const Navigation::TriangleNode& graphTriangle : System::Get<NavigationSystem>().GetMapTriangles())
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

        std::vector<Navigation::TriangleNode> tris = Navigation::BowyerWatson(Points);

        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            unsigned int idx;
            bool blocked;
            ss >> idx >> blocked;
            tris[idx].SetBlocked(blocked);
        }

        file.close();

        for(const Navigation::TriangleNode& graphTriangle : tris)
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

        System::Get<NavigationSystem>().SetMapTriangles(tris);
    }
}