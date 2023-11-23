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
                if(effect.isReady()) {
                    AttributesComponent& target = Game::GetRegistry().get<AttributesComponent>(effect.target);
                    AttributesComponent& source = Game::GetRegistry().get<AttributesComponent>(effect.source);
                    effect.callback(target, source);
                    effect.reset();
                }
            }
        }
        ac.effects.erase(std::remove_if(ac.effects.begin(), ac.effects.end(), removeEffectCallback), ac.effects.end());
    }
}

void AttributeSystem::OnEffect(const Effect &effect) {
    AttributesComponent& target = Game::GetRegistry().get<AttributesComponent>(effect.target);
    AttributesComponent& source = Game::GetRegistry().get<AttributesComponent>(effect.source);
    if(effect.type == INSTANT) {
        effect.callback(target, source);
    } else if(effect.type == DURATION) {
        target.effects.push_back(effect);
    }
}
