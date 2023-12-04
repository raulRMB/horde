#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "systems/System.h"

namespace tZ
{

class SParticle final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};

}

#endif //PARTICLE_SYSTEM_H
