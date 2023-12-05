#include "Player.h"
#include "raymath.h"
#include "components/Follow.h"
#include "util/Util.h"
#include "Particles.h"
#include "abilities/Projectile.h"
#include "systems/moba/Navigation.h"
#include "components/Animation.h"
#include "components/Model.h"
#include "networking/NetworkDriver.h"
#include "components/Network.h"

Player::Player()
{
    if(!Game::IsServer()) {
        CAnimation animation{};
        CModel mc = {LoadModel("../assets/anim.glb"), 0.05, false};
        mc.model.transform = MatrixRotateX(PI/2);
        animation.Animations = LoadModelAnimations("../assets/anim.glb", &animation.AnimsCount);
        animation.AnimsIndex = 1;
        animation.bPlaying = true;
        AddComponent(animation);
        AddComponent(mc);
    } else {
        CNetwork n = CNetwork{};
        Game::GetRegistry().emplace<CNetwork>(GetEntity(), n);
        NetworkDriver::GetNetworkedEntities().Add(GetEntity());
    }

    Transform transform{};


    std::list<FAttribute> attributes;
    FAttribute Health = {
            .id="health",
            .base=500,
            .max=1000,
            .min=0,
    };
    attributes.push_back(Health);
    FAttribute MoveSpeed = {
            .id="moveSpeed",
            .base=1,
            .max=500,
            .min=0,
    };
    attributes.push_back(MoveSpeed);
    CAttributes ac{attributes};

    OnApply healthRegenCallback = [](CAttributes& target, CAttributes& source){

        if(FAttribute* health = Util::GetAttribute(target, "health"); health)
        {
            float newHealth = health->base + 5;
            health->base = std::clamp(newHealth, health->min, health->max);
        }
    };
    FEffect healthRegen = FEffect{};
    healthRegen.type = INFINITE;
    healthRegen.target = GetEntity();
    healthRegen.source = GetEntity();
    healthRegen.callback = healthRegenCallback;
    healthRegen.rate = 3;


    CFollow follow{};
    follow.Index = 0;
    follow.FollowState = EFollowState::Idle;
    follow.Goal = {transform.translation.x, transform.translation.z};


    CPhysics2D physics{};
    physics.Speed = 9.f;
    physics.MaxSpeed = 9.f;
    physics.CollisionType = ECollision2DType::Circle;
    physics.CollisionRadius = 1.5f;
    AddComponent(transform);
    AddComponent(ac);
    AddComponent(physics);
    AddComponent(follow);

    Game::GetDispatcher().trigger(healthRegen);
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
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
//             CFollow& followComponent = GetComponent<CFollow>();
//             followComponent.FollowState = EFollowState::Dirty;
//             followComponent.Index = 1;
//             followComponent.Goal = mousePos;
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(IsKeyPressed(KEY_Q))
    {
        if(!Game::IsServer()) {
            RayCollision Collision = Util::GetMouseCollision();
            Transform clickPoint = Transform{Collision.point.x, 0.0f, Collision.point.z};
            Transform t = GetComponent<Transform>();
            Projectile(GetEntity(), clickPoint.translation, t.translation);
        }
    }
    if(IsKeyPressed(KEY_W))
    {
        OnApply effectCallback = [](CAttributes& target, CAttributes& source) {
            FAttribute& health = *Util::GetAttribute(target, "health");
            float newHealth = health.base - (0.2f * health.max);
            health.base = std::clamp(newHealth, health.min, health.max);
        };
        FEffect effect = FEffect{};
        effect.type = INSTANT;
        effect.target = GetEntity();
        effect.source = GetEntity();
        effect.callback = effectCallback;
        Game::GetDispatcher().trigger(effect);
    }
    if(IsKeyPressed(KEY_E))
    {
        FEffect effect = FEffect{};
        OnApply effectCallback = [&effect](CAttributes& target, CAttributes& source) {
            FAttribute& moveSpeed = *Util::GetAttribute(target, "moveSpeed");
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

}

void Player::Kill()
{
    SmartEntity::Kill();
}
