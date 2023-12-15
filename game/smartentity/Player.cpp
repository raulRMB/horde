#include "Player.h"
#include "components/Follow.h"
#include "util/Util.h"
#include "systems/moba/Navigation.h"
#include "components/Model.h"
#include "networking/base/NetworkDriver.h"
#include "components/Network.h"
#include "components/Physics.h"
#include "components/Attribute.h"
#include "components/Shapes.h"
#include "components/RayCollision.h"
#include "components/Animation.h"
#include "components/CharacterAnimation.h"
#include "components/Mortal.h"
#include "systems/SEnemy.h"

namespace tZ
{
static raylib::Font font;

Player::Player()
{
    if(!Game::IsServer()) {
        CAnimation animation{};
        CModel mc = {raylib::LoadModel("../assets/models/playerCharacter.glb"), 0.05, false};
        animation.Animations = raylib::LoadModelAnimations("../assets/models/playerCharacter.glb", &animation.AnimsCount);
        animation.AnimsIndex = (i32)ECharacterAnimState::Idle;
        animation.bPlaying = true;
        AddComponent(animation);
        AddComponent(mc);
        CNetwork n = CNetwork{};
        Game::GetRegistry().emplace<CNetwork>(GetEntity(), n);
    } else {
        CNetwork n = CNetwork{};
        Game::GetRegistry().emplace<CNetwork>(GetEntity(), n);
        NetworkDriver::GetNetworkedEntities().Add(GetEntity());
    }

    CTransform transform{};
    transform.Scale = v3{0.055, 0.055, 0.055};

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
            float newHealth = health->base + 15;
            health->base = std::clamp(newHealth, health->min, health->max);
        }
    };
    FEffect healthRegen = FEffect{};
    healthRegen.type = EEffectType::Infinite;
    healthRegen.target = GetEntity();
    healthRegen.source = GetEntity();
    healthRegen.callback = healthRegenCallback;
    healthRegen.rate = 1;

    CFollow follow{};
    follow.Index = 0;
    follow.FollowState = EFollowState::Idle;
    follow.Goal = {transform.Position.x, transform.Position.z};

    CPhysics2D physics{};
    physics.Speed = 9.f;
    physics.MaxSpeed = 9.f;
    physics.CollisionType = ECollision2DType::Circle;
    physics.CollisionRadius = 1.5f;

    CCharacterAnimation characterAnim{};
    characterAnim.AnimState = ECharacterAnimState::Idle;
    
    AddComponent(characterAnim);
    AddComponent(transform);
    AddComponent(ac);
    AddComponent(physics);
    AddComponent(follow);
    AddComponent(CMortal{});
    if(Game::IsServer()) {
        Game::GetDispatcher().trigger(healthRegen);
    }
}

Player::~Player() = default;

void Player::Start()
{
    System::Get<SEnemy>().SetPlayer(GetEntity());
}

void Player::HandleInput(entt::registry* Registry)
{
    if(raylib::IsMouseButtonDown(raylib::MOUSE_BUTTON_RIGHT)) {
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsMouseButtonPressed(raylib::MOUSE_BUTTON_RIGHT))
    {
        auto e = Registry->create();
        CMoveCircle mc = CMoveCircle {};
        mc.Position = Util::GetMouseWorldPosition();
        mc.Radius = 2.2;

        Registry->emplace<CMoveCircle>(e, mc);
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsKeyPressed(raylib::KEY_Q))
    {
        if(!Game::IsServer()) {
            FRayCollision Collision = Util::GetMouseCollision();
            auto vec = v3(Collision.point.x, 0.0f, Collision.point.z);
            auto netId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
            NetworkDriver::GetClient()->TriggerAbility(netId, 0, vec);
//            CTransform clickPoint = CTransform{vec};
//            CTransform t = GetComponent<CTransform>();
//            Projectile(GetEntity(), clickPoint.Position, t.Position);
        }
    }
    if(raylib::IsKeyPressed(raylib::KEY_W))
    {
    }
    if(raylib::IsKeyPressed(raylib::KEY_E))
    {
    }
    if(raylib::IsKeyPressed(raylib::KEY_R))
    {
    }
}

void Player::DrawUI() {}

void Player::Kill()
{
    SmartEntity::Kill();
}

void Player::SetTransform(CTransform &t) {
    CTransform& x = GetComponent<CTransform>();
    CNetwork& n = GetComponent<CNetwork>();
    n.TargetTransform = t;
    x.Position = t.Position;
    x.Scale = t.Scale;
    x.Rotation = t.Rotation;
}

}