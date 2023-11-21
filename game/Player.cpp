#include "Player.h"
#include "raymath.h"
#include "../core/components/FollowComponent.h"
#include "../core/components/Components.h"
#include "../core/util/Util.h"

Player::Player()
{
    AnimationComponent animation{};
    ModelComponent mc = {LoadModel("../assets/anim.glb"), 0.05, false};
    mc.model.transform = MatrixRotateX(PI/2);
    animation.Animations = LoadModelAnimations("../assets/anim.glb", &animation.AnimsCount);
    animation.AnimsIndex = 1;
    animation.bPlaying = true;
    AddComponent(animation);
    AddComponent(mc);

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

void Player::DrawUI() {
    Transform & t = GetComponent<Transform>();
    Vector2 healthBarPos = GetWorldToScreen(
            (Vector3){ t.translation.x, t.translation.y + 10.0f, t.translation.z },
            Game::Instance().GetActiveCamera());
    float maxHealth = 1000;
    float currHealth = 800;
    DrawRectangle(healthBarPos.x, healthBarPos.y, 80, 10, GRAY);
    DrawRectangle(healthBarPos.x, healthBarPos.y, 80 * currHealth/maxHealth, 10, GREEN);
    DrawTextEx(font, "Username", Vector2{healthBarPos.x, healthBarPos.y - 13}, 13, 1, WHITE);
}

void Player::Kill()
{
    SmartEntity::Kill();
}
