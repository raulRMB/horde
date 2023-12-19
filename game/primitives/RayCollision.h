#ifndef F_RAYCOLLISION_H
#define F_RAYCOLLISION_H

#include "util/defines.h"

namespace tX
{

typedef struct FRayCollision
{
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    v3 point;          // Point of the nearest hit
    v3 normal;         // Surface normal of hit
} FRayCollision;

}

#endif //RAYCOLLISION_H
