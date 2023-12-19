#include "SCharacterAnimation.h"
#include "components/CAnimation.h"
#include "components/CCharacterAnimation.h"
#include "components/CPhysics.h"

namespace tX
{

void SCharacterAnimation::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CPhysics2D, CCharacterAnimation>())
    {
        CPhysics2D& physics = GetComponent<CPhysics2D>(entity);
        CCharacterAnimation& characterAnim = GetComponent<CCharacterAnimation>(entity);

        if(characterAnim.bOverrideAnim)
        {
            DecideOverrideAnimation(deltaSeconds, characterAnim);
        }
        else
        {
            DecideBaseAnimation(characterAnim, physics);
        }
    }
}

void SCharacterAnimation::ResetAttackAnimation(CCharacterAnimation &characterAnim)
{
    switch(characterAnim.AnimState)
    {
        case ECharacterAnimState::Attacking1:
        case ECharacterAnimState::Attacking2:
        case ECharacterAnimState::Attacking3:
        case ECharacterAnimState::Attacking4:
            characterAnim.EndAnimTime = -1.f;
            characterAnim.CurrentAnimTime = 0.f;
            characterAnim.bOverrideAnim = false;
            break;
        default:
            break;
    }
}

void SCharacterAnimation::DecideBaseAnimation(CCharacterAnimation& characterAnim, CPhysics2D& physics)
{
    f32 speed = abs(glm::length(physics.Velocity));

    if(speed > 0.1f && characterAnim.AnimState != ECharacterAnimState::Walking)
    {
        characterAnim.AnimState = ECharacterAnimState::Walking;
        characterAnim.bNeedsNetSync = true;
    }
    else if(speed <= 0.1f && characterAnim.AnimState != ECharacterAnimState::Idle)
    {
        characterAnim.AnimState = ECharacterAnimState::Idle;
        characterAnim.bNeedsNetSync = true;
    }
}

void SCharacterAnimation::DecideOverrideAnimation(f32 deltaSeconds, CCharacterAnimation& characterAnim)
{
    characterAnim.CurrentAnimTime += deltaSeconds;

    if(characterAnim.CurrentAnimTime >= characterAnim.EndAnimTime && characterAnim.EndAnimTime > 0.f)
    {
        ResetAttackAnimation(characterAnim);
    }
}

}
