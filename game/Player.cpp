#include "Player.h"
#include "raymath.h"
#include "../core/Components.h"
#include "../core/util/Util.h"

Player::Player()
{
    CapsuleComponent capsule{};
    capsule.Radius = 0.5f;
    capsule.Height = 1.0f;
    capsule.Color = GREEN;
    AddComponent(capsule);
    AddComponent(FollowComponent{});
    AddComponent(Transform{});

    Physics2DComponent physics{};
    physics.Speed = 9.f;
    physics.MaxSpeed = 9.f;
    AddComponent(physics);
}

Player::~Player() = default;

void Player::Start()
{
}

void Player::HandleInput()
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        FollowComponent& followComponent = GetComponent<FollowComponent>();
        followComponent.bFollow = true;
        followComponent.Index = 1;
        followComponent.TargetPos = Util::GetMouseWorldPosition2D();
    }
}

void Player::Kill()
{
    SmartEntity::Kill();
}
