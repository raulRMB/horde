#include "Player.h"
#include "../core/Components.h"

Player::Player()
{
    AddComponent(Model{LoadModel("assets/anim.glb")});
    AnimationComponent animationComponent{};
    animationComponent.AnimsIndex = 1;
    animationComponent.Animations = LoadModelAnimations("assets/anim.glb", &animationComponent.AnimsCount);
    AddComponent(animationComponent);
    AddComponent(Transform{});
}

Player::~Player() = default;

void Player::Start()
{
}

void Player::Kill()
{
    SmartEntity::Kill();
}
