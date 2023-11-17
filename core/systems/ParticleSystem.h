#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "System.h"

class ParticleSystem final : public System
{
public:
    void Update(float deltaSeconds) override;
};


#endif
