#include "Player.h"
#include "raymath.h"
#include "../core/components/FollowComponent.h"
#include "../core/components/Components.h"
#include "../core/util/Util.h"
#include "Particles.h"

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

    std::vector<Attribute> attributes;
    Attribute Health = {
            .id="health",
            .value=500,
            .max=1000,
            .min=0,
    };
    attributes.push_back(Health);
    Attribute MoveSpeed = {
            .id="moveSpeed",
            .value=1,
            .max=500,
            .min=0,
    };
    attributes.push_back(MoveSpeed);
    AttributesComponent ac{attributes};
    AddComponent(ac);

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

void Player::HandleInput(entt::registry* Registry)
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        FollowComponent& followComponent = GetComponent<FollowComponent>();
        followComponent.FollowState = EFollowState::Dirty;
        followComponent.Index = 1;
        followComponent.Goal = Util::GetMouseWorldPosition2D();
    }
    if(IsKeyPressed(KEY_Q))
    {
        Effect effect;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.attribute = "health";
        effect.type = INSTANT;
        effect.operation = ADD;
        effect.value = 100;
        Game::GetDispatcher().trigger(effect);

        RayCollision Collision = Util::GetMouseCollision();
        Transform clickPoint = Transform{Collision.point.x, 0.0f, Collision.point.z};

        auto e = Registry->create();
        Registry->emplace<EmitterComponent>(e, EmitterComponent{.Frequency=0.01, .MaxParticles=100, .spawner=spawnParticle});
        Physics3DComponent phc = {};
        phc.Velocity = Vector3Normalize(clickPoint.translation - GetComponent<Transform>().translation) * 50;
        phc.MaxSpeed = 100;
        phc.Acceleration = Vector3 {1, 1, 1};
        Registry->emplace<Physics3DComponent>(e,phc);

        Registry->emplace<Transform>(e, GetComponent<Transform>().translation);
    }
    if(IsKeyPressed(KEY_W))
    {
        Effect effect;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.attribute = "health";
        effect.type = INSTANT;
        effect.operation = MULTIPLY;
        effect.value = 0.8;
        Game::GetDispatcher().trigger(effect);
    }
    if(IsKeyPressed(KEY_E))
    {
        Effect effect;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.attribute = "moveSpeed";
        effect.type = INSTANT;
        effect.operation = MULTIPLY;
        effect.value = 2;
        Game::GetDispatcher().trigger(effect);
    }
    if(IsKeyPressed(KEY_R))
    {
    }
}

void Player::DrawUI() {
    Transform & t = GetComponent<Transform>();
    Vector2 healthBarPos = GetWorldToScreen(
            (Vector3){ t.translation.x, t.translation.y + 10.0f, t.translation.z },
            Game::Instance().GetActiveCamera());
    AttributesComponent ac = GetComponent<AttributesComponent>();
    Attribute health = Util::GetAttribute(ac, "health");
    if (health.id != "empty") {
        DrawRectangle(healthBarPos.x, healthBarPos.y, 80, 10, GRAY);
        DrawRectangle(healthBarPos.x, healthBarPos.y, 80 * health.value/health.max, 10, GREEN);
        DrawTextEx(font, "Username", Vector2{healthBarPos.x, healthBarPos.y - 13}, 13, 1, WHITE);
    }
}

void Player::Kill()
{
    SmartEntity::Kill();
}
