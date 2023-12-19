#include "doctest.h"
#include "systems/base/Lifetime.h"
#include "components/Lifetime.h"
#include "components/Physics.h"
#include "components/Follow.h"
#include "systems/moba/Navigation.h"
#include "systems/moba/Follow.h"
#include "systems/base/Physics.h"
#include "components/Enemy.h"
#include "systems/EnemySystem.h"
#include <entt/entt.hpp>

namespace tZ {

    TEST_CASE("LifeTime_System_Test1") {
        entt::registry r;
        entt::entity e = r.create();

        auto lt = CLifetime{};
        lt.MaxLifetime = 3.0;

        r.emplace<CLifetime>(e, lt);
        auto LifetimeSystem = System::Get<SLifetime>();

        LifetimeSystem.Process(e, 1.9, r);

        auto res = r.try_get<CLifetime>(e);
        CHECK(res != nullptr);
        CHECK(r.valid(e));
    }

    TEST_CASE("LifeTime_System_Test2") {
        entt::registry r;
        entt::entity e = r.create();

        auto lt = CLifetime{};
        lt.MaxLifetime = 3;

        r.emplace<CLifetime>(e, lt);
        auto LifetimeSystem = System::Get<SLifetime>();

        LifetimeSystem.Process(e, 3.2, r);

        auto res = r.try_get<CLifetime>(e);
        CHECK(res == nullptr);
        CHECK(!r.valid(e));
    }

    TEST_CASE("Enemy_Follow_System_Integration_Test") {
        entt::registry r;
        entt::entity e = r.create();
        auto navSystem = System::Get<SNavigation>();
        navSystem.LoadNavMesh(r);
        v2 goal = v2{20, 20};

        if(navSystem.IsValidPoint(goal)) {
            auto transform = CTransform{};
            r.emplace<CTransform>(e, transform);

            auto pc = CPhysics2D{};
            pc.MaxSpeed = 10;
            pc.Speed = 10;
            r.emplace<CPhysics2D>(e, pc);

            auto ec = CEnemy{};
            r.emplace<CEnemy>(e, ec);

            auto fc = CFollow{};
            r.emplace<CFollow>(e, fc);

            auto followSystem = System::Get<SFollow>();
            auto physicsSystem = System::Get<SPhysics>();
            auto enemySystem = System::Get<EnemySystem>();

            auto player = r.create();

            auto pt = CTransform{};
            pt.Position = {goal.x, 0, goal.y};
            r.emplace<CTransform>(player, pt);

            enemySystem.SetPlayer(player);
            enemySystem.Process(e, r, 1);

            float deltaSeconds = 0.001;

            int movePlayer = 0;

            while (r.get<CFollow>(e).FollowState != EFollowState::Idle) {
                enemySystem.Process(e, r, deltaSeconds);
                navSystem.Process(e, r, deltaSeconds);
                followSystem.Process(e, r, deltaSeconds);
                physicsSystem.Process2D(e, r, deltaSeconds);
                auto t = r.get<CTransform>(e);
                printf("%.2f, %.2f, %.2f\n", t.Position.x, t.Position.y, t.Position.z);
                movePlayer++;
                if(movePlayer % 150 == 0) {
                    CTransform& tp = r.get<CTransform>(player);
                    tp.Position += 0.2;
                    movePlayer = 0;
                }
            }
            auto t = r.get<CTransform>(e);
            auto ffc = r.get<CFollow>(e);
            CHECK(t.Position == v3{ffc.Goal.x, 0, ffc.Goal.y});
        }
    }
}