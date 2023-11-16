#include "Player.h"
#include "raymath.h"
#include "../core/Components.h"

Player::Player()
{
    // Model model = LoadModel("assets/anim.glb");
    Transform transform{};
    transform.translation = {10.0, 0.0, 10.0};
    transform.scale = {0.1, 0.1, 0.1};
    transform.rotation= {1.0, 0.0, 0.0, 1.0};
    // model.transform = MatrixRotateXYZ({PI/2,0.f,0.f});
    // AddComponent(model);
    // AnimationComponent animationComponent{};
    // animationComponent.AnimsIndex = 0;
    // animationComponent.Animations = LoadModelAnimations("assets/anim.glb", &animationComponent.AnimsCount);
    // AddComponent(animationComponent);
    AddComponent(transform);
    CapsuleComponent capsuleComponent{};
    capsuleComponent.Position = transform.translation;
    capsuleComponent.Radius = 0.5f;
    capsuleComponent.Height = 1.0f;
    capsuleComponent.Color = GRAY;

    AddComponent(capsuleComponent);
}

Player::~Player() = default;

void Player::Start()
{
}

void Player::Kill()
{
    SmartEntity::Kill();
}
