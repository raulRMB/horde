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

enum EffectType {
    INSTANT  = 0,
    DURATION,
    INFINITE
};

enum EffectOperation {
    ADD = 0,
    MULTIPLY
};

struct Effect {
    entt::entity target;
    entt::entity source;
    std::string attribute;
    EffectType type;
    EffectOperation operation;
    float value;
};

struct Attribute {
    std::string id;
    float value;
    float max;
    float min;
    std::vector<Effect*> effects;
};

struct AttributesComponent {
    std::vector<Attribute> attributes;
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