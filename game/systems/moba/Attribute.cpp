#include "Attribute.h"
#include "entt/entt.hpp"
#include "Game.h"

namespace tZ
{

void SAttribute::Init()
{
    Game::GetDispatcher().sink<FEffect>().connect<&SAttribute::OnEffect>();
}

auto removeEffect = [](FEffect& effect) {
    CAttributeSet& ac = Game::GetRegistry().get<CAttributeSet>(effect.target);
    for(FAttribute& attr : ac.Attributes) {
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

void SAttribute::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity :registry.view<CAttributeSet>())
    {
        CAttributeSet& ac = registry.get<CAttributeSet>(entity);
        for (auto it = ac.Effects.begin(); it != ac.Effects.end();) {
            FEffect& effect = *it;
            if(effect.isExpired() && effect.type == EEffectType::Duration) {
                removeEffect(effect);
                it = ac.Effects.erase(it);
            } else {
                if(effect.isReady()) {
                    CAttributeSet& target = Game::GetRegistry().get<CAttributeSet>(effect.target);
                    CAttributeSet& source = Game::GetRegistry().get<CAttributeSet>(effect.source);
                    effect.callback(target, source);
                    effect.reset();
                    ac.bNeedsNetSync = true;
                }
                effect.addElapsed(deltaSeconds);
                ++it;
            }
        }
    }
}

void SAttribute::OnEffect(const FEffect &effect) {
    CAttributeSet& target = Game::GetRegistry().get<CAttributeSet>(effect.target);
    CAttributeSet& source = Game::GetRegistry().get<CAttributeSet>(effect.source);
    if(effect.type == EEffectType::Instant) {
        effect.callback(target, source);
        target.bNeedsNetSync = true;
    } else if(effect.type == EEffectType::Duration || effect.type == EEffectType::Infinite) {
        if(effect.callOnInit) {
            effect.callback(target, source);
        }
        target.Effects.push_back(effect);
    }
}

}