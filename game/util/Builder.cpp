#include "Builder.h"

#include "util/defines.h"
#include "components/CFollow.h"
#include "util/Util.h"
#include "components/CModel.h"
#include "networking/base/NetworkDriver.h"
#include "components/CNetwork.h"
#include "components/CPhysics.h"
#include "components/CAttribute.h"
#include "components/CAnimation.h"
#include "components/CCharacterAnimation.h"
#include "systems/SEnemy.h"
#include "components/CSpawner.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "components/CShapes.h"
#include "components/CEnemy.h"

namespace tX
{

    entt::entity Builder::Player(CTransform& t) {
        entt::entity e = Game::GetRegistry().create();
        Game::GetRegistry().emplace<CTransform>(e, t);
        System::Get<SEnemy>().SetPlayer(e);

        if(!Game::IsServer()) {
            CAnimation animation{};
            CModel mc = {raylib::LoadModel("../assets/models/playerCharacter.glb"), 0.05, false};
            animation.Animations = raylib::LoadModelAnimations("../assets/models/playerCharacter.glb", &animation.AnimsCount);
            animation.AnimsIndex = (i32)ECharacterAnimState::Idle;
            animation.bPlaying = true;
            Game::GetRegistry().emplace<CAnimation>(e, animation);
            Game::GetRegistry().emplace<CModel>(e, mc);
        }

        if(!Game::IsStandalone()) {
            CNetwork n = CNetwork{};
            n.TargetTransform = t;
            Game::GetRegistry().emplace<CNetwork>(e, n);
            if(Game::IsServer()) {
                NetworkDriver::GetNetworkedEntities().Add(e);
            }
        }

        std::list<FAttribute> attributes;
        FAttribute Health = {
                .id="health",
                .base=500,
                .max=1000,
                .min=0,
        };
        attributes.push_back(Health);
        FAttribute MoveSpeed = {
                .id="moveSpeed",
                .base=1,
                .max=500,
                .min=0,
        };
        attributes.push_back(MoveSpeed);
        CAttributeSet ac{attributes};

        OnApply healthRegenCallback = [](CAttributeSet& target, CAttributeSet& source){

            if(FAttribute* health = Util::GetAttribute(target, "health"); health)
            {
                float newHealth = health->base + 15;
                health->base = std::clamp(newHealth, health->min, health->max);
            }
        };
        FEffect healthRegen = FEffect{};
        healthRegen.type = EEffectType::Infinite;
        healthRegen.target = e;
        healthRegen.source = e;
        healthRegen.callback = healthRegenCallback;
        healthRegen.rate = 1;

        CFollow follow{};
        follow.Index = 0;
        follow.FollowState = EFollowState::Idle;
        follow.Goal = {t.Position.x, t.Position.z};

        CPhysics2D physics{};
        physics.Speed = 9.f;
        physics.MaxSpeed = 9.f;
        physics.CollisionType = ECollision2DType::Circle;
        physics.CollisionRadius = 1.5f;

        CCharacterAnimation characterAnim{};
        characterAnim.AnimState = ECharacterAnimState::Idle;

        Game::GetRegistry().emplace<CCharacterAnimation>(e, characterAnim);
        Game::GetRegistry().emplace<CAttributeSet>(e, ac);
        Game::GetRegistry().emplace<CPhysics2D>(e, physics);
        Game::GetRegistry().emplace<CFollow>(e, follow);
        if(Game::IsServer()) {
            Game::GetDispatcher().trigger(healthRegen);
        }
        return e;
    }

    entt::entity Builder::Enemy(CTransform& t) {
        entt::registry& registry = Game::GetRegistry();
        entt::entity e = registry.create();
        registry.emplace<CTransform>(e, t);

        if(!Game::IsStandalone()) {
            CNetwork net = CNetwork{};
            net.TargetTransform = t;
            registry.emplace<CNetwork>(e, net);
        }

        if(!Game::IsServer()) {
            CSphere sphereComponent{};
            sphereComponent.Color = FColor(0xFF0000FF);
            sphereComponent.Radius = .5f;
            registry.emplace<CSphere>(e, sphereComponent);
        }

        CFollow followComp{};
        registry.emplace<CFollow>(e, followComp);
        CPhysics2D physics{};
        physics.Speed = 10.f;
        physics.MaxSpeed = 10.f;
        registry.emplace<CPhysics2D>(e, physics);
        CEnemy enemy{};
        registry.emplace<CEnemy>(e, enemy);
        std::list<FAttribute> attributes;
        FAttribute Health = {
                .id="health",
                .base=500,
                .max=1000,
                .min=0,
        };
        attributes.push_back(Health);
        CAttributeSet ac{attributes};
        registry.emplace<CAttributeSet>(e, ac);
        return e;
    }

    entt::entity Builder::Spawner() {
        entt::entity spawner = Game::GetRegistry().create();
        CTransform d = CTransform {};
        d.Position = v3{15, 0, 15};
        CSpawner s = CSpawner{2};
        s.SpawnRate = 5;
        Game::GetRegistry().emplace<CTransform>(spawner, d);
        Game::GetRegistry().emplace<CSpawner>(spawner, s);
        return spawner;
    }

    entt::entity Builder::Map() {
        entt::entity e = Game::GetRegistry().create();
        CTransform t = {};
        t.Rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
        t.Scale = v3(250.f);
        Game::GetRegistry().emplace<CTransform>(e, t);
        raylib::Model m = raylib::LoadModelFromMesh(raylib::GenMeshPlane(1, 1, 1, 1));
        m.materials[0].maps[raylib::MATERIAL_MAP_DIFFUSE].texture = raylib::LoadTexture("../assets/textures/arena.png");
        Game::GetRegistry().emplace<CModel>(e, CModel(m, 1.0f, false));
        return e;
    }

}
