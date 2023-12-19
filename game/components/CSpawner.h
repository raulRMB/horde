#ifndef SPAWNER_COMPONENT_H
#define SPAWNER_COMPONENT_H

#include "components/tX_Component.h"

namespace tX
{

struct CSpawner : CComponent
{
    float SpawnTimer;
    float SpawnRate;

    CSpawner(float spawnRate);
};

}

#endif // SPAWNER_COMPONENT_H