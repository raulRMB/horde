#include <entt/entt.hpp>
#include "util/Util.h"
#include "util/raymathEx.h"
#include "components/Particle.h"
#include "components/Shapes.h"

Spawner spawnParticle2 = [](entt::entity e, Transform& transform, entt::registry& registry, CParticle pc) {

    pc.MaxLife = 0.3;

    CCapsule cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    CPhysics3D phc = {};
    phc.Velocity = Util::RandVec3(-50, 50);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-20, 20);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<CParticle>(e, pc);
    registry.emplace<CCapsule>(e, cc);
    registry.emplace<CPhysics3D>(e, phc);
    registry.emplace<Transform>(e, t);
};