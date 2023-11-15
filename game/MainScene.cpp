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
    mainCanvas->Add(new Button());
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
