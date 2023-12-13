#ifndef HORDE_LIFETIME_H
#define HORDE_LIFETIME_H

#include "systems/System.h"

namespace tZ
{

    class SLifetime : public UpdateSystem {
    public:
        void Update(float deltaSeconds) override;
    };

}

#endif //HORDE_LIFETIME_H
