#ifndef C_PROJECTILE_H
#define C_PROJECTILE_H

#include "components/TechXComponent.h"
#include "util/defines.h"
#include "entt/entt.hpp"

namespace tZ
{

struct CProjectile : CComponent
{
    f32 Range = 0.0f;
    bool bIsHoming = false;
    bool bIsPiercing = false;
    bool bIsBouncing = false;
    bool bShouldSpawnOnHit = false;
    entt::entity* Target = nullptr;
};

}

#endif //C_PROJECTILE_H
