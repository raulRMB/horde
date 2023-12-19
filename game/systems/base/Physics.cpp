//
// Created by Raul on 11/17/2023.
//

#include "Physics.h"
#include "components/Physics.h"

#include "entt/entt.hpp"
#include "components/Attribute.h"
#include "components/Transform.h"
#include "util/raymathEx.h"
#include "util/Util.h"

namespace tZ
{
std::set<entt::entity> toDestroy;

void SPhysics::Update(float deltaSeconds)
{
    ClearAffectedEntities();

    entt::registry& registry = Game::GetRegistry();
    // for(const entt::entity& entity : registry.view<Transform, CPhysics2D, CAttributes>())
    // {
    //     Transform& transform = registry.get<Transform>(entity);
    //     CAttributes& ac = registry.get<CAttributes>(entity);
    //     FAttribute* moveSpeed = Util::GetAttribute(ac, "moveSpeed");

    //     if(moveSpeed->id != "empty") {
    //         CPhysics2D &physics = registry.get<CPhysics2D>(entity);
    //         physics.Velocity += physics.Acceleration * deltaSeconds;
    //         physics.Velocity = Vector2ClampValue(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
    //         Vector3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
    //         velocity *= moveSpeed->get();
    //         transform.translation += velocity * deltaSeconds;
    //     }
    // }

    auto entities = registry.view<CPhysics2D, CTransform>();
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        AddAffectedEntity(*it);
        for (auto jt = std::next(it); jt != entities.end(); ++jt) {
            AddAffectedEntity(*jt);
            entt::entity x = *it;
            entt::entity y = *jt;
            CPhysics2D &xP = entities.get<CPhysics2D>(x);
            CTransform &xT = entities.get<CTransform>(x);
            CPhysics2D &yP = entities.get<CPhysics2D>(y);
            CTransform &yT = entities.get<CTransform>(y);
            if(Util::Check2DCollision(xP, xT, yP, yT)) {
                auto ac = registry.try_get<CAttributeSet>(y);
                auto eff = registry.try_get<FEffect>(x);
                if(ac != nullptr && eff != nullptr && eff->source != y) {
                    eff->target = y;
                    Game::GetDispatcher().trigger(*eff);
                    toDestroy.insert(x);
                }
                if(!Game::IsServer() && eff != nullptr && eff->source != y) {
                    toDestroy.insert(x);
                }
            }
        }
    }

    for(entt::entity x : toDestroy) {
        auto found = registry.try_get<CTransform>(x);
        if(found != nullptr) {
            RemoveAffectedEntity(x);
            registry.destroy(x);
        }
    }
    toDestroy.clear();

    for(const entt::entity& entity : registry.view<CPhysics2D, CTransform>())
    {
        AddAffectedEntity(entity);
        CTransform& transform = registry.get<CTransform>(entity);
        CPhysics2D& physics = registry.get<CPhysics2D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = glm::clamp(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        v3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
        transform.Position += velocity * deltaSeconds;
    }
    for(const entt::entity& entity : registry.view<CTransform, CPhysics3D>())
    {
        AddAffectedEntity(entity);
        CTransform& transform = registry.get<CTransform>(entity);
        CPhysics3D& physics = registry.get<CPhysics3D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = glm::clamp(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        transform.Position += physics.Velocity * deltaSeconds;
    }
}


}