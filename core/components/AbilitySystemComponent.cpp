#include "AbilitySystemComponent.h"

Effect::Effect(std::string name, EffectType type, OnApply callback, entt::entity target, entt::entity source) {
    this->id = RandomInt();
    this->name = std::move(name);
    this->type = type;
    this->callback = callback;
    this->target = target;
    this->source = source;
}

AttributesComponent& Effect::getAttributesComponent(entt::entity e) {
    entt::registry& registry = Game::GetRegistry();
    return registry.get<AttributesComponent>(e);
}

void Effect::trigger(entt::registry& registry) {
    elapsedSince = 0;
    callback(getAttributesComponent(target), getAttributesComponent(source));
}
