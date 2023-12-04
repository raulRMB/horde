#include <entt/entt.hpp>
#include "util/Util.h"
#include "components/Particle.h"
#include "components/Physics.h"
#include "components/Shapes.h"
#include "components/Transform.h"

namespace tZ
{

// Spawner spawnParticle2 = [](entt::entity e, CTransform& transform, entt::registry& registry, CParticle pc)
// {
//
//     pc.MaxLife = 0.3;
//
//     CCapsule cc = {};
//     cc.Color = Util::RandColor();
//     cc.Height = 0.5;
//     cc.Position = v3(0.f);
//     cc.Radius = 0.5;
//     cc.Slices = 1;
//
//     CPhysics3D phc = {};
//     phc.Velocity = Util::RandVec3(-50, 50);
//     phc.MaxSpeed = 100;
//     phc.Acceleration = Util::RandVec3(-20, 20);
//
//     CTransform t = {};
//     t.Position = Util::RandVec3(-1, 1);
//     t.Position = transform.Position + t.Position;
//
//     registry.emplace<CParticle>(e, pc);
//     registry.emplace<CCapsule>(e, cc);
//     registry.emplace<CPhysics3D>(e, phc);
//     registry.emplace<CTransform>(e, t);
// };

}
