#include "Lifetime.h"
#include "components/Lifetime.h"

namespace tZ
{
    void SLifetime::Update(float deltaSeconds) {
        entt::registry& registry = Game::GetRegistry();
        for(const entt::entity& entity : registry.view<CLifetime>())
        {
            CLifetime& lt = registry.get<CLifetime>(entity);
            lt.CurrentLifetime += deltaSeconds;
            if(lt.CurrentLifetime >= lt.MaxLifetime) {
                registry.destroy(entity);
            }
        }
    }
}