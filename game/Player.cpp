#include "Player.h"
#include "raymath.h"
#include "../core/components/FollowComponent.h"
#include "../core/components/Components.h"
#include "../core/util/Util.h"

Player::Player()
{
    CapsuleComponent capsule{};
    capsule.Radius = 0.5f;
    capsule.Height = 1.0f;
    capsule.color = GREEN;
    AddComponent(capsule);

    Transform transform{};
    AddComponent(transform);

    FollowComponent follow{};
    follow.Index = 0;
    follow.FollowState = EFollowState::Idle;
    follow.Goal = {transform.translation.x, transform.translation.z};
    AddComponent(follow);

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
        followComponent.FollowState = EFollowState::Dirty;
        followComponent.Index = 1;
        followComponent.Goal = Util::GetMouseWorldPosition2D();
    }
}

void Player::Kill()
{
    SmartEntity::Kill();
}
