//
// Created by Raul on 12/7/2023.
//

#include "Projectile.h"

tZ::CProjectile::CProjectile(tZ::f32 range, bool isHoming, bool isPiercing, bool isBouncing, bool shouldSpawnOnHit,
                             entt::entity *target)
    : Range(range), bIsHoming(isHoming), bIsPiercing(isPiercing), bIsBouncing(isBouncing),
        bShouldSpawnOnHit(shouldSpawnOnHit), Target(target), CComponent("Projectile")
{
}
