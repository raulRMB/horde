#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
#include <vector>
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

struct Attribute {
    std::string id;
    float value;
    float max;
    float min;
};

using OnApply = void (*)(AttributesComponent&, AttributesComponent&);
class Effect {
public:
    entt::entity target;
    entt::entity source;
    EffectType type;
    OnApply callback;
    float rate;
    float duration;
    bool isExpired() {
        return elapsedLifetime > duration;
    }
    bool isReady() {
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

struct AttributesComponent {
    std::vector<Attribute> attributes;
    std::vector<Effect> effects;
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