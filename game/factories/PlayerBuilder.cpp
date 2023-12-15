#include "PlayerBuilder.h"

#include "components/Animation.h"
#include "components/Attribute.h"
#include "components/CharacterAnimation.h"
#include "components/Follow.h"
#include "components/Model.h"
#include "components/Mortal.h"
#include "components/Network.h"
#include "components/Physics.h"
#include "networking/base/NetworkDriver.h"
#include "util/Util.h"

namespace tZ
{

entt::entity BPlayer::Build(const CTransform& transform)
{
    StartBuilding();

    NetworkDriver::GetNetworkedEntities().Add(GetCurrentEntity());

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
    healthRegen.target = GetCurrentEntity();
    healthRegen.source = GetCurrentEntity();
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

entt::entity BPlayer::Build(const CTransform& transform, u32 netId)
{
    CAnimation animation{};
    CModel mc = {raylib::LoadModel("../assets/models/playerCharacter.glb"), 0.05, false};
    animation.Animations = raylib::LoadModelAnimations("../assets/models/playerCharacter.glb", &animation.AnimsCount);
    animation.AnimsIndex = (i32)ECharacterAnimState::Idle;
    animation.bPlaying = true;
    AddComponent(animation);
    AddComponent(mc);
}

}
