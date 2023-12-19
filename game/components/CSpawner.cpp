#include "CSpawner.h"

namespace tX
{

CSpawner::CSpawner(float spawnRate) : SpawnRate(spawnRate), SpawnTimer(0.f), CComponent("Spawner")
{
}

} // namespace tX