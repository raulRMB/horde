#include "Animation.h"
#include "entt/entt.hpp"
#include "components/Animation.h"
#include "components/Model.h"
#include "Game.h"
#include "components/CharacterAnimation.h"

namespace raylib
{
#include "raylib.h"
}

namespace tZ
{

void SAnimation::Update(float deltaSeconds)
{
    for(const entt::entity& entity : Game::GetRegistry().view<CAnimation, CModel>())
    {
        CAnimation& animationComponent = Game::GetRegistry().get<CAnimation>(entity);
        const raylib::Model& model = Game::GetRegistry().get<CModel>(entity).model;

        raylib::ModelAnimation animation = animationComponent.Animations[animationComponent.AnimsIndex];

        animationComponent.CurrentFrameTime += 60.f * deltaSeconds;

        if(animationComponent.CurrentFrameTime >= 1.0f)
        {
            animationComponent.CurrentFrameTime = 0.0f;
            animationComponent.CurrentFrame++;

            if(animationComponent.CurrentFrame > animation.frameCount - 1)
            {
                animationComponent.CurrentFrame = 0;
            }
        }

        raylib::UpdateModelAnimation(model, animation, animationComponent.CurrentFrame);
    }

    for(const entt::entity& entity : Game::GetRegistry().view<CAnimation, CCharacterAnimation>())
    {
        CAnimation& animationComponent = Game::GetRegistry().get<CAnimation>(entity);
        CCharacterAnimation& characterAnimation = Game::GetRegistry().get<CCharacterAnimation>(entity);
        if(animationComponent.AnimsIndex != (int)characterAnimation.AnimState) {
            animationComponent.Reset((int)characterAnimation.AnimState);
        }
    }
}

}