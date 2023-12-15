//
// Created by Raul on 11/17/2023.
//

#include "Physics.h"
#include "components/Physics.h"

#include "GarbageManager.h"
#include "entt/entt.hpp"
#include "components/Attribute.h"
#include "components/Transform.h"
#include "util/raymathEx.h"
#include "util/Util.h"

namespace tZ
{

void SPhysics::Update(float deltaSeconds)
{
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
        for (auto jt = std::next(it); jt != entities.end(); ++jt) {
            entt::entity x = *it;
            entt::entity y = *jt;
            CPhysics2D &xP = entities.get<CPhysics2D>(x);
            CTransform &xT = entities.get<CTransform>(x);
            CPhysics2D &yP = entities.get<CPhysics2D>(y);
            CTransform &yT = entities.get<CTransform>(y);
            if(Util::Check2DCollision(xP, xT, yP, yT)) {
                SGarbageManager& garbageMan = System::Get<SGarbageManager>();
                CAttributes* ac = registry.try_get<CAttributes>(y);
                FEffect* eff = registry.try_get<FEffect>(x);
                if(ac != nullptr && eff != nullptr && eff->source != y) {
                    eff->target = y;
                    Game::GetDispatcher().trigger(*eff);
                    FAttribute* health = Util::GetAttribute(*ac, "health");

                    if(health != nullptr && health->get() <= 0.f)
                    {
                        garbageMan.Add(y);
                    }
                    garbageMan.Add(x);

                }
                if(!Game::IsServer() && eff != nullptr && eff->source != y) {
                    garbageMan.Add(x);
                }
            }
        }
    }
    for(entt::entity entity : registry.view<CPhysics2D, CTransform>())
    {
        CTransform& transform = registry.get<CTransform>(entity);
        CPhysics2D& physics = registry.get<CPhysics2D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = glm::clamp(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        v3 velocity = {physics.Velocity.x, 0.f, physics.Velocity.y};
        transform.Position += velocity * deltaSeconds;
    }
    for(entt::entity entity : registry.view<CTransform, CPhysics3D>())
    {
        CTransform& transform = registry.get<CTransform>(entity);
        CPhysics3D& physics = registry.get<CPhysics3D>(entity);
        physics.Velocity += physics.Acceleration * deltaSeconds;
        physics.Velocity = glm::clamp(physics.Velocity, -physics.MaxSpeed, physics.MaxSpeed);
        transform.Position += physics.Velocity * deltaSeconds;
    }
}

}