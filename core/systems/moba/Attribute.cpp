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
    CAttributes& ac = Game::GetRegistry().get<CAttributes>(effect.target);
    for(FAttribute& attr : ac.attributes) {
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
    for(const entt::entity& entity :registry.view<CAttributes>())
    {
        CAttributes& ac = registry.get<CAttributes>(entity);
        for (auto it = ac.effects.begin(); it != ac.effects.end();) {
            FEffect& effect = *it;
            if(effect.isExpired() && effect.type == EEffectType::Duration) {
                removeEffect(effect);
                it = ac.effects.erase(it);
            } else {
                if(effect.isReady()) {
                    CAttributes& target = Game::GetRegistry().get<CAttributes>(effect.target);
                    CAttributes& source = Game::GetRegistry().get<CAttributes>(effect.source);
                    effect.callback(target, source);
                    effect.reset();
                    ac.needsSync = true;
                }
                effect.addElapsed(deltaSeconds);
                ++it;
            }
        }
    }
}

void SAttribute::OnEffect(const FEffect &effect) {
    CAttributes& target = Game::GetRegistry().get<CAttributes>(effect.target);
    CAttributes& source = Game::GetRegistry().get<CAttributes>(effect.source);
    if(effect.type == EEffectType::Instant) {
        effect.callback(target, source);
        target.needsSync = true;
    } else if(effect.type == EEffectType::Duration || effect.type == EEffectType::Infinite) {
        if(effect.callOnInit) {
            effect.callback(target, source);
        }
        target.effects.push_back(effect);
    }
}

}