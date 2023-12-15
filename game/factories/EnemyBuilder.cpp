#include "EnemyBuilder.h"

#include "components/EnemyComponent.h"
#include "components/Network.h"
#include "components/Shapes.h"
#include "networking/base/NetworkDriver.h"

namespace tZ
{

entt::entity BEnemy::Build(const CTransform& transform)
{
    StartBuilding();
    AddComponent(transform);
    AddComponent(CEnemy{});
    AddComponent(CNetwork{});

    return FinishBuilding();
}

entt::entity BEnemy::Build(const CTransform& transform, u32 netId)
{
    StartBuilding();
    AddComponent(transform);
    AddComponent(CEnemy{});
    AddComponent(CNetwork{});
    CSphere sphere{};
    sphere.Radius = 1.f;
    sphere.Color = FColor(0xFF0000FF);
    AddComponent(sphere);

    NetworkDriver::GetNetworkedEntities().Add(GetCurrentEntity(), netId);

    return FinishBuilding();
}
}
