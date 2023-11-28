#include "Player.h"
#include "raymath.h"
#include "../core/components/FollowComponent.h"
#include "../core/components/Components.h"
#include "../core/util/Util.h"
#include "Particles.h"
#include "abilities/Projectile.h"
#include "systems/moba/NavigationSystem.h"

Player::Player()
{
    if(!Game::IsServer()) {
        font = LoadFont("../assets/ui/fonts/Roboto-Bold.ttf");
        AnimationComponent animation{};
        ModelComponent mc = {LoadModel("../assets/anim.glb"), 0.05, false};
        mc.model.transform = MatrixRotateX(PI/2);
        animation.Animations = LoadModelAnimations("../assets/anim.glb", &animation.AnimsCount);
        animation.AnimsIndex = 1;
        animation.bPlaying = true;
        AddComponent(animation);
        AddComponent(mc);
    }

    Transform transform{};
    AddComponent(transform);

    std::list<Attribute> attributes;
    Attribute Health = {
            .id="health",
            .base=500,
            .max=1000,
            .min=0,
    };
    attributes.push_back(Health);
    Attribute MoveSpeed = {
            .id="moveSpeed",
            .base=1,
            .max=500,
            .min=0,
    };
    attributes.push_back(MoveSpeed);
    AttributesComponent ac{attributes};
    AddComponent(ac);

    OnApply healthRegenCallback = [](AttributesComponent& target, AttributesComponent& source){

        if(Attribute* health = Util::GetAttribute(target, "health"); health)
        {
            float newHealth = health->base + 5;
            health->base = std::clamp(newHealth, health->min, health->max);
        }
    };
    Effect healthRegen = Effect{};
    healthRegen.type = INFINITE;
    healthRegen.target = GetEntity();
    healthRegen.source = GetEntity();
    healthRegen.callback = healthRegenCallback;
    healthRegen.rate = 1;
    Game::GetDispatcher().trigger(healthRegen);

    FollowComponent follow{};
    follow.Index = 0;
    follow.FollowState = EFollowState::Idle;
    follow.Goal = {transform.translation.x, transform.translation.z};
    AddComponent(follow);

    Physics2DComponent physics{};
    physics.Speed = 9.f;
    physics.MaxSpeed = 9.f;
    physics.CollisionType = Circle;
    physics.CollisionRadius = 1.5f;
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
        Vector2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<NavigationSystem>().IsValidPoint(mousePos))
        {
            TraceLog(LOG_INFO, "x: %f, y: %f", mousePos.x, mousePos.y);
            if(Game::IsOfflineMode()) {
                FollowComponent& followComponent = GetComponent<FollowComponent>();
                followComponent.FollowState = EFollowState::Dirty;
                followComponent.Index = 1;
                followComponent.Goal = mousePos;
            } else {
                Game::GetClient()->SendMoveTo(mousePos);
            }
        }
    }
    if(IsKeyPressed(KEY_Q))
    {
        RayCollision Collision = Util::GetMouseCollision();
        Transform clickPoint = Transform{Collision.point.x, 0.0f, Collision.point.z};
        Transform t = GetComponent<Transform>();
        Projectile(GetEntity(), clickPoint.translation, t.translation);
    }
    if(IsKeyPressed(KEY_W))
    {
        OnApply effectCallback = [](AttributesComponent& target, AttributesComponent& source) {
            Attribute& health = *Util::GetAttribute(target, "health");
            float newHealth = health.base - (0.2f * health.max);
            health.base = std::clamp(newHealth, health.min, health.max);
        };
        Effect effect = Effect{};
        effect.type = INSTANT;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.callback = effectCallback;
        Game::GetDispatcher().trigger(effect);
    }
    if(IsKeyPressed(KEY_E))
    {
        Effect effect = Effect{};
        OnApply effectCallback = [&effect](AttributesComponent& target, AttributesComponent& source) {
            Attribute& moveSpeed = *Util::GetAttribute(target, "moveSpeed");
            AttrMod moveSpeedMod = AttrMod{
                    effect.id,
                    [&moveSpeed](float x) -> float {
                        float newVal = std::clamp(x * 3.50f, moveSpeed.min, moveSpeed.max);
                        return newVal;
                    }
            };
            moveSpeed.mods.push_back(moveSpeedMod);
        };
        effect.type = DURATION;
        effect.duration = 4;
        effect.rate = -1;
        effect.callOnInit = true;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.callback = effectCallback;
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
    Attribute health = *Util::GetAttribute(ac, "health");
    if (health.id != "empty") {
        DrawRectangle(healthBarPos.x, healthBarPos.y, 80, 10, GRAY);
        DrawRectangle(healthBarPos.x, healthBarPos.y, 80 * health.get()/health.max, 10, GREEN);
        DrawTextEx(font, "Username", Vector2{healthBarPos.x, healthBarPos.y - 13}, 13, 1, WHITE);
    }
}

void Player::Kill()
{
    SmartEntity::Kill();
}
