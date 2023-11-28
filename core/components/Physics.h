#ifndef HORDE_PHYSICS_H
#define HORDE_PHYSICS_H

#include <raylib.h>

enum class ECollision2DType
{
    Circle = 0,
    Rect
};

struct CPhysics2D
{
    float Speed = 0.f;
    float MaxSpeed = 0.f;

    ECollision2DType CollisionType;
    float CollisionRadius;
    Rectangle CollisionBox;

    Vector2 Velocity{ };
    Vector2 Acceleration{};
};

struct CPhysics3D
{
    float Speed = 0.f;
    float MaxSpeed = 0.f;
    Vector3 Velocity{ };
    Vector3 Acceleration{};
};

#endif //HORDE_PHYSICS_H
