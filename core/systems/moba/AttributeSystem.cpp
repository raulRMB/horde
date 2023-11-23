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

auto removeEffect = [](Effect& effect) {
    AttributesComponent& ac = Game::GetRegistry().get<AttributesComponent>(effect.target);
    for(Attribute& attr : ac.attributes) {
        for (auto it = attr.mods.begin(); it != attr.mods.end();) {
            const AttrMod& mod = *it;
            if (mod.effectId == effect.id) {
                it = attr.mods.erase(it);
            } else {
                ++it;
            }
        }
    }
};

void AttributeSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity :registry.view<AttributesComponent>())
    {
        AttributesComponent& ac = registry.get<AttributesComponent>(entity);
        for (auto it = ac.effects.begin(); it != ac.effects.end();) {
            Effect& effect = *it;
            if(effect.isExpired() && effect.type == DURATION) {
                removeEffect(effect);
                it = ac.effects.erase(it);
            } else {
                if(effect.isReady()) {
                    AttributesComponent& target = Game::GetRegistry().get<AttributesComponent>(effect.target);
                    AttributesComponent& source = Game::GetRegistry().get<AttributesComponent>(effect.source);
                    effect.callback(target, source);
                    effect.reset();
                }
                effect.addElapsed(deltaSeconds);
                ++it;
            }
        }
    }
}

void AttributeSystem::OnEffect(const Effect &effect) {
    AttributesComponent& target = Game::GetRegistry().get<AttributesComponent>(effect.target);
    AttributesComponent& source = Game::GetRegistry().get<AttributesComponent>(effect.source);
    if(effect.type == INSTANT) {
        effect.callback(target, source);
    } else if(effect.type == DURATION || effect.type == INFINITE) {
        if(effect.callOnInit) {
            effect.callback(target, source);
        }
        target.effects.push_back(effect);
    }
}
