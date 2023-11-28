#ifndef HORDE_ATTRIBUTE_H
#define HORDE_ATTRIBUTE_H

#include <list>
#include <algorithm>
#include <string>
#include <limits>
#include <raylib.h>
#include <entt/entt.hpp>

class FEffect;
struct CAttributes;

enum EEffectType {
    INSTANT  = 0,
    DURATION,
    INFINITE
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
        for(AttrMod mod : mods) {
            x = mod.callback(x);
        }
        return x;
    }
};

using OnApply = std::function<void(CAttributes&, CAttributes&)>;
class FEffect
{
public:
    int id = GetRandomValue(0, std::numeric_limits<int>::max());
    entt::entity target;
    entt::entity source;
    EEffectType type;
    OnApply callback;
    bool callOnInit;
    float rate;
    float duration;
    bool isExpired() {
        if(type != DURATION) {
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

struct CAttributes
{
    std::list<FAttribute> attributes;
    std::list<FEffect> effects;
};

#endif //HORDE_ATTRIBUTE_H
