#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <list>
#include <string>
#include <entt/entt.hpp>

struct AnimationComponent
{
    int CurrentFrame;
    int AnimsCount;
    int AnimsIndex;
    bool bPlaying;
    float CurrentFrameTime;
    float FrameTime;
    ModelAnimation* Animations;
};

struct ParticleComponent 
{
    float MaxLife;
    float Lifetime;
    entt::entity emitterId = entt::null;
};

struct ModelComponent
{
    Model model;
    float scale;
    bool hidden;
};

typedef void (*Spawner)(entt::entity, Transform&, entt::registry&, ParticleComponent);

struct EmitterComponent 
{
    float Frequency;
    float Time;
    int MaxParticles;
    int ParticleCount;
    Spawner spawner;
};

struct TargetComponent
{
    Vector3 Position;
};

/* Attribute/GameplayEffect */

class Effect;
struct AttributesComponent;

enum EffectType {
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

struct Attribute {
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

using OnApply = std::function<void(AttributesComponent&, AttributesComponent&)>;
class Effect {
public:
    int id = GetRandomValue(0, INT_MAX);
    entt::entity target;
    entt::entity source;
    EffectType type;
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

inline bool operator==(const Effect& x, const Effect& y)
{
    return x.id == y.id;
}

struct AttributesComponent {
    std::list<Attribute> attributes;
    std::list<Effect> effects;
};

/* End Attribute/GameplayEffect */

struct EnemyComponent
{
    bool bAlive;
};

struct CubeComponent
{
    Vector3 Position;
    Vector3 Size;
    Color color;
};

struct CapsuleComponent
{
    Vector3 Position;
    float Radius;
    float Height;
    int Slices;
    Color color;
};

struct SphereComponent
{
    Vector3 Position;
    float Radius;
    Color color;
};

struct RayComponent
{
    Ray ray;
    Color color;
};

struct TriangleComponent
{
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Color color;
};

struct LineStripComponent
{
    std::vector<Vector3> Points;
    Color color;
};

struct Physics2DComponent
{
    float Speed = 0.f;
    float MaxSpeed = 0.f;
    Vector2 Velocity{ };
    Vector2 Acceleration{};
};

struct Physics3DComponent
{
    float Speed = 0.f;
    float MaxSpeed = 0.f;
    Vector3 Velocity{ };
    Vector3 Acceleration{};
};

#endif //COMPONENTS_H