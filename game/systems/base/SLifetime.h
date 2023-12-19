#ifndef HORDE_SLIFETIME_H
#define HORDE_SLIFETIME_H

#include "systems/System.h"

namespace tX
{

    class SLifetime : public UpdateSystem {
    public:
        void Update(float deltaSeconds) override;
        void Process(const entt::entity& entity, float deltaSeconds, entt::registry& r);
    };

}

#endif //HORDE_SLIFETIME_H
