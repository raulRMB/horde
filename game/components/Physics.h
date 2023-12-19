#ifndef HORDE_PHYSICS_H
#define HORDE_PHYSICS_H

#include "Rectangle.h"
#include "components/TechXComponent.h"

namespace tZ
{

enum class ECollision2DType
{
    Circle = 0,
    Rect
};

struct CPhysics2D : CComponent
{
    float Speed;
    float MaxSpeed;

    ECollision2DType CollisionType;
    float CollisionRadius;
    CRectangle CollisionBox;

    v2 Velocity;
    v2 Acceleration;

    CPhysics2D(float speed = 0.f, float maxSpeed = 0.f, ECollision2DType collisionType = ECollision2DType::Circle,
               float collisionRadius = 0.f, CRectangle collisionBox = CRectangle(), v2 velocity = v2(),
               v2 acceleration = v2());
};

struct CPhysics3D : CComponent
{
    float Speed;
    float MaxSpeed;
    v3 Velocity;
    v3 Acceleration;

    CPhysics3D(float speed = 0.f, float maxSpeed = 0.f, v3 velocity = v3(), v3 acceleration = v3());
};

}

#endif //HORDE_PHYSICS_H
