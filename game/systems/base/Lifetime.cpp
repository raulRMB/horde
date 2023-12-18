#include "Lifetime.h"
#include "components/Lifetime.h"

namespace tZ
{

    void SLifetime::Update(float deltaSeconds) {
        entt::registry& registry = Game::GetRegistry();
        for(const entt::entity& entity : registry.view<CLifetime>())
        {
            Process(entity, deltaSeconds, registry);
        }
    }

    void SLifetime::Process(const entt::entity& entity, float deltaSeconds, entt::registry& r) {
        CLifetime& lt = r.get<CLifetime>(entity);
        lt.CurrentLifetime += deltaSeconds;
        if(lt.CurrentLifetime >= lt.MaxLifetime) {
            r.destroy(entity);
        }
    }

}