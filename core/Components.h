#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>

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

struct TargetComponent
{
    Vector3 Position;
};

struct FollowComponent
{
    entt::entity Target;
};

struct HealthComponent
{
    float Health;
};

struct EnemyComponent
{
    bool bAlive;
};

struct CubeComponent
{
    Vector3 Position;
    Vector3 Size;
    Color Color;
};

struct CapsuleComponent
{
    Vector3 Position;
    float Radius;
    float Height;
    int Slices;
    Color Color;
};

struct SphereComponent
{
    Vector3 Position;
    float Radius;
    Color Color;
};

#endif //COMPONENTS_H