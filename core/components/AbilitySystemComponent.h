#ifndef HORDE_ABILITYSYSTEMCOMPONENT_H
#define HORDE_ABILITYSYSTEMCOMPONENT_H

#include <vector>
#include <random>
#include <string>
#include "../util/Util.h"
#include <entt/entt.hpp>

int RandomInt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    int randomId = distribution(gen);
    return randomId;
}

class Effect;
class Attribute;

struct AttributesComponent {
    std::vector<Attribute> attributes;
    std::vector<Effect> effects;
};

enum EffectType {
    INSTANT  = 0,
    DURATION,
    INFINITE
};

class Attribute {
public:
    std::string id;
    float mod;
    float value;
    float max;
    float min;
    std::vector<Effect*> effects;
    float get() const {
        return mod + value;
    }
};

using OnApply = void (*)(AttributesComponent&, AttributesComponent&);
class Effect {
public:
    Effect(std::string name, EffectType type, OnApply callback, entt::entity target, entt::entity source);
    int getId() { return id; }
    void start() {
        Game::GetRegistry().get<AttributesComponent>(target).effects.push_back(this);
    }
    EffectType getType() { return type; }
    void setDurationInfo(float rate, float duration) {
        this->rate = rate;
        this->duration = duration;
    }
    bool isExpired() {
        return elapsedSince > elapsedLifetime;
    }
    AttributesComponent& getAttributesComponent(entt::entity e);
    void trigger(entt::registry& registry);
    bool ready() {
        return elapsedSince >= rate;
    }
    void addElapsed(float deltaSeconds) {
        elapsedSince += deltaSeconds;
        elapsedLifetime += deltaSeconds;
    }
private:
    int id;
    std::string name;
    EffectType type;
    OnApply callback{};
    entt::entity target;
    entt::entity source;
    float rate{};
    float elapsedSince{};
    float elapsedLifetime{};
    float duration{};
};

#endif //HORDE_ABILITYSYSTEMCOMPONENT_H
