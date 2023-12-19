#ifndef C_PROJECTILE_H
#define C_PROJECTILE_H

#include "components/tX_Component.h"
#include "util/defines.h"
#include "entt/entt.hpp"

namespace tX
{

struct CProjectile : CComponent
{
    f32 Range;
    bool bIsHoming;
    bool bIsPiercing;
    bool bIsBouncing;
    bool bShouldSpawnOnHit;
    entt::entity* Target;

    CProjectile(f32 range = 0.0f, bool isHoming = false, bool isPiercing = false, bool isBouncing = false,
                bool shouldSpawnOnHit = false, entt::entity* target = nullptr);
};

}

#endif //C_PROJECTILE_H
