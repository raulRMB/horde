#include <raylib.h>
#include "MainScene.h"
#include "Player.h"
#include "../core/ui/elements/button/Button.h"

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
}

void MainScene::Update(float deltaTime)
{
    mainCanvas->Update();
    Scene::Update(deltaTime);
}

void MainScene::Draw()
{
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
