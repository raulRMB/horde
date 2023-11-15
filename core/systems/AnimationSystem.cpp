#include "AnimationSystem.h"
#include <entt/entt.hpp>
#include "../Components.h"
#include "../../game/Game.h"

void AnimationSystem::Update(float deltaSeconds)
{
    for(const entt::entity& entity : Game::GetRegistry().view<AnimationComponent, Model>())
    {
        AnimationComponent& animationComponent = Game::GetRegistry().get<AnimationComponent>(entity);
        const Model& model = Game::GetRegistry().get<Model>(entity);

        ModelAnimation animation = animationComponent.Animations[animationComponent.AnimsIndex];

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