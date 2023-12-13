#ifndef HORDE_PHYSICS_H
#define HORDE_PHYSICS_H

#include "Rectangle.h"

namespace tZ
{

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
    CRectangle CollisionBox;

    v2 Velocity{};
    v2 Acceleration{};
};

struct CPhysics3D
{
    float Speed = 0.f;
    float MaxSpeed = 0.f;
    v3 Velocity{ };
    v3 Acceleration{};
};

}

#endif //HORDE_PHYSICS_H
