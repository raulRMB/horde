#include <entt/entt.hpp>
#include "components/Components.h"
#include "../core/util/Util.h"
#include "../core/util/raymathEx.h"

Spawner spawnParticle = [](entt::entity e, Transform& transform, entt::registry& registry, ParticleComponent pc) {

    pc.MaxLife = 0.2;

    CapsuleComponent cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    Physics3DComponent phc = {};
    phc.Velocity = Util::RandVec3(-2, 2);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-1, 1);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<ParticleComponent>(e, pc);
    registry.emplace<CapsuleComponent>(e, cc);
    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, t);
};

Spawner spawnParticle2 = [](entt::entity e, Transform& transform, entt::registry& registry, ParticleComponent pc) {

    pc.MaxLife = 0.3;

    CapsuleComponent cc = {};
    cc.color = Util::RandColor();
    cc.Height = 0.5;
    cc.Position = Vector3{0,0,0};
    cc.Radius = 0.5;
    cc.Slices = 1;

    Physics3DComponent phc = {};
    phc.Velocity = Util::RandVec3(-50, 50);
    phc.MaxSpeed = 100;
    phc.Acceleration = Util::RandVec3(-20, 20);

    Transform t = {};
    t.translation = Util::RandVec3(-1, 1);
    t.translation = transform.translation + t.translation;

    registry.emplace<ParticleComponent>(e, pc);
    registry.emplace<CapsuleComponent>(e, cc);
    registry.emplace<Physics3DComponent>(e, phc);
    registry.emplace<Transform>(e, t);
};