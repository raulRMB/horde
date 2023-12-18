#include "doctest.h"
#include "systems/base/Lifetime.h"
#include "components/Lifetime.h"
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
}