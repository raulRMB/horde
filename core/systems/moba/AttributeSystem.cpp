#include "AttributeSystem.h"
#include "entt/entt.hpp"
#include "components/Components.h"
#include "Game.h"
#include "util/Util.h"
#include <algorithm>

void AttributeSystem::Init()
{
    Game::GetDispatcher().sink<Effect>().connect<&AttributeSystem::OnEffect>();
}

auto removeEffectCallback = [](Effect& effect) {
    return effect.isExpired();
};

void AttributeSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity :registry.view<AttributesComponent>())
    {
        AttributesComponent& ac = registry.get<AttributesComponent>(entity);
        for(Effect& effect : ac.effects) {
            if(!effect.isExpired()) {
                effect.addElapsed(deltaSeconds);
                if(effect.ready()) {
                    effect.trigger(registry);
                }
            }
        }
        ac.effects.erase(std::remove_if(ac.effects.begin(), ac.effects.end(), removeEffectCallback), ac.effects.end());
    }
}

void AttributeSystem::OnEffect(Effect &effect) {
    switch(effect.getType()) {
        case INSTANT:
            effect.trigger(Game::GetRegistry());
            break;
        case DURATION:
            effect.start();
            break;
    }
}
