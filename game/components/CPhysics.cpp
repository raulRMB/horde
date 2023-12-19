#include "CPhysics.h"

namespace tX
{

CPhysics2D::CPhysics2D(float speed, float maxSpeed, ECollision2DType collisionType, float collisionRadius,
                       CRectangle collisionBox, v2 velocity, v2 acceleration)
    : Speed(speed), MaxSpeed(maxSpeed), CollisionType(collisionType), CollisionRadius(collisionRadius),
        CollisionBox(collisionBox), Velocity(velocity), Acceleration(acceleration), CComponent("Physics2D")
{}

CPhysics3D::CPhysics3D(float speed, float maxSpeed, v3 velocity, v3 acceleration)
    : Speed(speed), MaxSpeed(maxSpeed), Velocity(velocity), Acceleration(acceleration), CComponent("Physics3D")
{}

}