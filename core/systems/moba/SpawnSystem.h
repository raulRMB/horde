#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "systems/System.h"

class SpawnSystem : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
};

#endif // SPAWN_SYSTEM_H