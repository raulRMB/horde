#include "Animation.h"
#include "entt/entt.hpp"
#include "components/Animation.h"
#include "components/Model.h"
#include "Game.h"
#include <raylib.h>
#include <algorithm>

namespace tZ
{

void SAnimation::Update(float deltaSeconds)
{
    for(const entt::entity& entity : Game::GetRegistry().view<CAnimation, CModel>())
    {
        CAnimation& animationComponent = Game::GetRegistry().get<CAnimation>(entity);
        const Model& model = Game::GetRegistry().get<CModel>(entity).model;

        ::ModelAnimation animation = animationComponent.Animations[animationComponent.AnimsIndex];

        animationComponent.CurrentFrameTime += 0.1f;

        if(animationComponent.CurrentFrameTime >= 1.0f)
        {
            animationComponent.CurrentFrameTime = 0.0f;
            animationComponent.CurrentFrame++;

            if(animationComponent.CurrentFrame > animation.frameCount - 1)
            {
                animationComponent.CurrentFrame = 0;
            }
        }

        UpdateModelAnimation(model, animation, animationComponent.CurrentFrame);
    }
}

}