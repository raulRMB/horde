#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
#include <vector>

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
    std::vector<Vector2> StringPath;
    Vector2 TargetPos;
    bool bFollow;
    int Index;
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

struct RayComponent
{
    Ray Ray;
    Color Color;
};

struct TriangleComponent
{
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Color Color;
};

struct LineStripComponent
{
    std::vector<Vector3> Points;
    Color Color;
};

#endif //COMPONENTS_H