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

#endif //COMPONENTS_H