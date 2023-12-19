#ifndef HORDE_CATTRIBUTE_H
#define HORDE_CATTRIBUTE_H

#include <list>
#include <algorithm>
#include <string>
#include <limits>
#include "entt/entt.hpp"
#include "Game.h"

namespace tX
{
    
class FEffect;
struct CAttributeSet;

enum class EEffectType {
    Instant  = 0,
    Duration,
    Infinite
};


using AttrModCallback = std::function<float(float)>;

struct AttrMod {
    int effectId;
    AttrModCallback callback;
};

inline bool operator==(const AttrMod& x, const AttrMod& y)
{
    return x.effectId == y.effectId;
}

struct FAttribute
{
    std::string id;
    float base;
    float max;
    float min;
    std::list<AttrMod> mods;
    float get() {
        float x = base;
        for (const AttrMod& mod: mods) {
            x = mod.callback(x);
        }
        return x;
    }
};

using OnApply = std::function<void(CAttributeSet&, CAttributeSet&)>;
class FEffect
{
public:
    int id = rand() % RAND_MAX;
    entt::entity target;
    entt::entity source;
    EEffectType type;
    OnApply callback;
    bool callOnInit;
    float rate;
    float duration;
    bool isExpired() {
        if(type != EEffectType::Duration) {
            return false;
        }
        return elapsedLifetime > duration;
    }
    bool isReady() {
        if(rate <= 0) {
            return false;
        }
        return elapsedSince >= rate;
    }
    void reset() {
        elapsedSince = 0;
    }
    void addElapsed(float deltaSeconds) {
        elapsedSince += deltaSeconds;
        elapsedLifetime += deltaSeconds;
    }
private:
    float elapsedSince;
    float elapsedLifetime;
};

inline bool operator==(const FEffect& x, const FEffect& y)
{
    return x.id == y.id;
}

struct CAttributeSet : CComponent
{
    std::list<FAttribute> Attributes;
    std::list<FEffect> Effects;

    CAttributeSet(const std::list<FAttribute>& attributes = {}, const std::list<FEffect>& effects = {}) :
        Attributes(attributes), Effects(effects), CComponent("AttributeSet") {}
};

}

#endif //HORDE_CATTRIBUTE_H
