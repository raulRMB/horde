#include "CProjectile.h"

namespace tX
{

CProjectile::CProjectile(f32 range, bool isHoming, bool isPiercing, bool isBouncing, bool shouldSpawnOnHit,
                             entt::entity *target)
    : Range(range), bIsHoming(isHoming), bIsPiercing(isPiercing), bIsBouncing(isBouncing),
        bShouldSpawnOnHit(shouldSpawnOnHit), Target(target), CComponent("Projectile")
{

}

}
